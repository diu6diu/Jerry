# SENT Protocol Stack Changes Documentation

## Directory Structure Changes

### Before:
```
MT95p1_Src/
├── 17000628_Communication/SENT/
│   ├── sentcald.c/h
│   ├── sentmect.c
│   ├── sentmegr.c
│   ├── sentmgpf.c
│   ├── sentmmng.c
│   ├── sentnvmd.c
│   └── sentpapi.h
├── 17000628_HAL/17000628_HWIO/
│   ├── io_sent.h
│   ├── sent_port.c/h
│   └── sent_device_driver.h
└── TC3xx_DeviceDrivers/TC_SENT/
    ├── dd_sent.c/h
    ├── hw_sent.h
    └── dd_sent_init.c/h
```

### After:
```
SENT/
├── application/
│   ├── sent_cald.c/h
│   ├── sent_mect.c
│   ├── sent_megr.c
│   ├── sent_mgpf.c
│   ├── sent_mmng.c
│   ├── sent_nvmd.c
│   └── sent_papi.h
├── hal/
│   ├── sent_port.c/h
│   ├── io_sent.h
│   └── sent_device_driver.h
├── driver/
│   ├── dd_sent.c/h
│   ├── hw_sent.h
│   └── dd_sent_init.c/h
└── docs/
    ├── architecture.puml
    ├── sequence.puml
    └── changes.md
```

## File Content Changes

### Header Files Changes

1. Copyright and Sensitive Information Removal:
```diff
- /******************************************************************************
- * Copyright (C) 20XX-20XX Delphi Corporation, All Rights Reserved.
- * MT95p1 Project Specific Code
- ******************************************************************************/
+ /******************************************************************************
+ * SENT Protocol Stack Implementation
+ ******************************************************************************/
```

2. Include Path Updates:
```diff
- #include "17000628_Communication/SENT/sent_papi.h"
+ #include "application/sent_papi.h"

- #include "17000628_HAL/17000628_HWIO/io_sent.h"
+ #include "hal/io_sent.h"

- #include "TC3xx_DeviceDrivers/TC_SENT/dd_sent.h"
+ #include "driver/dd_sent.h"
```

3. Macro Definitions Updates:
```diff
- #define MT95P1_SENT_VERSION    "1.0.0"
+ #define SENT_VERSION           "1.0.0"

- #define MT95P1_SENT_MAX_CHANNELS  10
+ #define SENT_MAX_CHANNELS         10
```

### Source Files Changes

1. Function Name Updates:
```diff
- MT95P1_SENT_Initialize()
+ SENT_Initialize()

- MT95P1_SENT_ProcessMessage()
+ SENT_ProcessMessage()
```

2. Variable Name Updates:
```diff
- mt95p1_sent_config_t
+ sent_config_t

- mt95p1_sent_status
+ sent_status
```

3. Comment Updates:
```diff
- /* MT95p1 specific SENT configuration */
+ /* SENT configuration */

- /* Company specific implementation */
+ /* Implementation specific */
```

## Architecture Documentation

1. Added PlantUML architecture diagram (architecture.puml)
2. Added PlantUML sequence diagram (sequence.puml)
3. Created changes documentation (this file)

## Functional Changes
- No functional changes were made
- All modifications were related to:
  - Directory structure reorganization
  - Removal of project-specific references
  - Standardization of naming conventions
  - Documentation updates
  - Include path adjustments

## Testing Recommendations
1. Verify all include paths are correct
2. Confirm all files compile without errors
3. Test all SENT communication functions
4. Validate protocol timing requirements
5. Check error handling scenarios

## Known Issues
- None. All changes are structural and naming-related only.