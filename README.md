# TP Shooter  


## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [License](#license)

##installation
1. You Clone the repository:
```bash
 git clone https://github.com/jorgemmm/UECPPShooter.git
```
You can download as zip.

2. Install dependencies:
```bash
 You need Visual studio 2019  and unreal engine 4.26
 You need setup visual Studio for Unreal engine 
 ```
[Setting VS for UE ](https://dev.epicgames.com/documentation/en-us/unreal-engine/setting-up-visual-studio-for-unreal-engine?application_version=4.27)

## usage

Es un ejemplo de cómo implementar usando punto de mira en un shooter
line tarce y soporte multijugador
Tambien gestiona las animaciones en CPP
  
  
```bash
bool TraceUnderCursor();//(FHitResult ScreenTracerHit);
void FXFire(FHitResult HitResult);//, FVector& BeamEndLocation);
UFUNCTION(Server, Reliable)
  void Server_FireWeapon(const FVector_NetQuantize& TraceHitTarget);
UFUNCTION(NetMulticast, Reliable)
  void Multicast_FireWeapon(const FVector_NetQuantize& TraceHitTarget);

```


## License
This project is licensed under the [MIT License](https://mit-license.org/).
