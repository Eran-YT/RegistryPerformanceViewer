# RegistryPerformanceViewer

Extract information from HKEY_PERFORMANCE_DATA, and output as csv files.

## Usage
### Help
```cmd
RegistryPerformanceViewer.exe -help
``` 

### List counters
```cmd
RegistryPerformanceViewer.exe list
``` 

### Dump specific counter
```cmd
RegistryPerformanceViewer.exe dump -c Process -c output_directory
``` 

### Dump all counters
```cmd
RegistryPerformanceViewer.exe dump_all
``` 
You may specify the output directory like so:
```cmd
RegistryPerformanceViewer.exe dump_all -o output_directory
``` 