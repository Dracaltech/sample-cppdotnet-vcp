# sample-cppdotnet-vcp
Dracal // SDK code sample for C++ (.NET) on VCP

## Assumptions

Running this repository requires you to have installed:
- .NET (version >= `8.0`)
- Visual Studio (version >= 2022)

## Simple usage

Run by
- Using the **Play** button (Visual Studio)
- Build and run using the command line:

```
dotnet run sample-cppdotnet-vcp.sln
```

## Sample output
<img src="https://github.com/Dracaltech/sample-cppdotnet-vcp/assets/1357711/4abf44bc-8085-4577-bdca-5e50bfd5ab5b" width=400 />

```
Awaiting info line...
Awaiting info line...
Printing 2 fractional digits
I,Product ID,Serial Number,Message,MS5611 Pressure,Pa,SHT31 Temperature,C,SHT31 Relative Humidity,%,*bbdd

2024-04-25 10:09 AM VCP-PTH450-CAL E24638
MS5611 Pressure           101267 Pa
SHT31 Temperature         22.97 C
SHT31 Relative Humidity   45.79 %

2024-04-25 10:09 AM VCP-PTH450-CAL E24638
MS5611 Pressure           101266 Pa
SHT31 Temperature         22.95 C
SHT31 Relative Humidity   45.83 %

2024-04-25 10:09 AM VCP-PTH450-CAL E24638
MS5611 Pressure           101265 Pa
SHT31 Temperature         22.95 C
SHT31 Relative Humidity   45.83 %

2024-04-25 10:09 AM VCP-PTH450-CAL E24638
MS5611 Pressure           101262 Pa
SHT31 Temperature         22.97 C
SHT31 Relative Humidity   45.81 %

C:\dev\dracal\sample-cppdotnet-vcp\x64\Debug\sample-cppdotnet-vcp.exe (process 36032) exited with code 0.
To automatically close the console when debugging stops, enable Tools->Options->Debugging->Automatically close the console when debugging stops.
Press any key to close this window . . .
```
