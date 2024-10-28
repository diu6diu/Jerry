# Flash 原理

Flash的内部存储是金属-氧化层-半导体-场效晶体管(MOSFET)，里面有个悬浮门(Floating Gate)，是真正存储数据的单元。

[内存单元](https://pic4.zhimg.com/v2-a2b809a6a3000a3b8d42a31c6c234a37_r.jpg)

数据在Flash内存单元中是以电荷(electrical charge) 形式存储的。存储电荷的多少，取决于图中的控制门（Control gate）所被施加的电压，其控制了是向存储单元中冲入电荷还是使其释放电荷。而数据的表示，以所存储的电荷的电压是否超过一个特定的阈值Vth 来表示。

1. 对于NAND Flash的写入（编程)，就是控制Control Gate去充电（对Control Gate加压），使得悬浮门存储的电荷够多，超过阈值Vth，就表示0。

2. 对于NAND Flash的擦除(Erase)，就是对悬浮门放电，低于阀值Vth，就表示1。

***由于Flash数据需要长期存储，Floating Gate必须具有强大的电荷保存能力，这意味着对 Floating Gate进行放电也就是擦除工作是非常缓慢的。综上，Flash的三大操作执行速度为读最快，写次之，擦除最慢。当Flash中的 block 数据有更新时，如果采用EEPROM的存储方式，需要先对 block 擦除，根据 block 大小不同， 这个时间可能达到 ms 级别，然后再进行写入，又将花费 us 级别的时间。 这个等待时长在高实时嵌入式系统中时不可接受的，因此引入 FEE 模块（FLASH EEPROM Emulation Module），对上层模块 NvM 隐藏FLASH特性。***

## Flash 特性

Flash Memory 具有质量轻、能耗低、体积小、抗震能力强等的优点，但也有不少局限性，主要如下：

1. **需要先擦除再写入**
Flash Memory 写入数据时有一定的限制。它只能将当前为 1 的比特改写为 0，而无法将已经为 0 的比特改写为 1，只有在擦除的操作中，才能把整块的比特改写为 1。
2. **块擦除次数有限**
Flash Memory 的每个数据块都有擦除次数的限制（十万到百万次不等），擦写超过一定次数后，该数据块将无法可靠存储数据，成为坏块。
为了最大化的延长 Flash Memory 的寿命，在软件上需要做擦写均衡（Wear Leveling），通过分散写入、动态映射等手段均衡使用各个数据块。同时，软件还需要进行坏块管理（Bad Block Management，BBM），标识坏块，不让坏块参与数据存储。（注：除了擦写导致的坏块外，Flash Memory 在生产过程也会产生坏块，即固有坏块。）
3. **读写干扰**
由于硬件实现上的物理特性，Flash Memory 在进行读写操作时，有可能会导致邻近的其他比特发生位翻转，导致数据异常。这种异常可以通过重新擦除来恢复。Flash Memory 应用中通常会使用 ECC 等算法进行错误检测和数据修正。
4. **电荷泄漏**
存储在 Flash Memory 存储单元的电荷，如果长期没有使用，会发生电荷泄漏，导致数据错误。不过这个时间比较长，一般十年左右。此种异常是非永久性的，重新擦除可以恢复。

### FEE 的机制以及解决的问题

1. **总是将数据写入已擦除的区域**
此机制解决了局限性1和2，即当某一个Block数据又更新时，将新数据直接写入到所有数据的后面。由于取消了擦除步骤因此提高了写入性能。

[顺序写入](https://pic3.zhimg.com/80/v2-dd77f07f1c28ddddaffb32c99f6d071a_720w.webp)

用一个表格简单演示一下。

    - 原有数据如下：

|flash head|
|---|
|block 1|
|block 2|
|blank|

    - block 1 数据有更新：

|flash head|
|---|
|block 1|
|block 2|
|block 1|

这个机制又带来了两个问题：

- 如何找到最新数据?
  在 RAM 中维护一张表格，就可以在O(1)的时间内找到每个block最新的数据。重启之后，从Flash的末尾往上倒着读，可以使用最短的时间初始化。
- Flash 写满之后怎么办? 也就是在什么时机进行擦除?
  这个涉及到GC机制，也就是 garbage collection，这个比较复杂，在下一段介绍。


2. **Garbage Collection**

垃圾回收主要解决的是局限性 2

GC 将整个物理 Flash 人为的分成 2 个 Sector。当一个 Sector A 在机制 1 下被写满之后，FEE 将此 Sector A 下所有的有效数据，也就最新数据 copy 到另外一个 Sector B，后续的新数据也都写到Sector B 中，并且在Background Task 中整个擦除 Secter A。因为擦除工作是在CPU空闲时完成的，所以擦除所消耗的时间对上层应用无感。当 Sector B 也被写满之后， A 已经被擦除了，可以把有效数据 copy 到 A 中，如此循环往复。

因为 GC 机制下， 整个Flash都会被均匀的写入和擦除，因此 GC 解决了局限性 2

[Garbage Collection](https://pic4.zhimg.com/80/v2-9416a9a4fb164a096d3734b4280dccaf_720w.webp)

此举又带来两个问题：

- 如何知道每个 Sector 的状态?
  这个问题涉及到FLASH data的数据组织形式，也就是 layout。后面再讲。
- 当 GC 机制激活之后，也就是在Copy 有效数据的时候，有一个很主要的 Block (Immediate Data) 需要存储的时候怎么办? 如果等待，所花费的时间会更长。
  引入 Threshold， 当 Sector 剩余空间小于 Threshold 时，就触发 GC 机制。 在GC过程中，有重要数据需要写入时，可暂时中断 GC ，将重要数据写入到 剩余空间，之后再重新开始 GC。


3. **在写入之前先读**

这个机制没有官方的名字，这里简单介绍一下。次机制是为了解决局限性 3

当写入的地址不是 Word Line 的开始地址，也就是是要写入的 word line 已经有数据了， 需要将已有的数据读出来，保存到ram， 然后再执行写入。 写完之后，再读出此 word line 中的数据，与ram中的备份对比。如果不相同，则将备份的数据写入到下一条word line，并将当前 word line 标记为err。

4. **FLASH Layout**

> 8字节的Header信息+每个Dflash页1byte的内部信息+每个DFlash页7字节数据+1页(8byte)标记信息

大致如此，各家实现不尽相同，贴一张图。

[Flash Layout](https://pic3.zhimg.com/80/v2-437941da667c22f12aa47f34f41e96c2_720w.webp)