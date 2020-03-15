# Getting Started

```bash
# Find and download the packages
mbed deploy

# Check dependencies
mbed ls
```

- Check the `compile.bat` and `flash.bat` to know which commands to run from mbed cli
- VSCode is used to add Intellisense


## NOTE

### FPU printf does not work

- Since we are using BLUETOOTH, Float is converted to Uint16
- `uint16_t val = (uint16_t)(fval100);`
- i.e -> 1.12 -> 112
