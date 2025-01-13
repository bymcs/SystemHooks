# SystemHooks (32-bit Internal DLL)

Bu proje, 32-bit Windows uygulamalarında çeşitli API fonksiyonlarını hook eden bir DLL'dir. Hook işlemleri process içinde (in-process) gerçekleşir.

## Desteklenen Hook'lar

1. GetAdaptersInfo Hook:
   - MAC adreslerini dinamik olarak değiştirir
   - Her çağrıda rastgele MAC adresleri üretir

2. Discord RPC Hook:
   - Discord_UpdatePresence fonksiyonunu hook eder
   - Discord durum güncellemelerini engeller veya değiştirir

## Gereksinimler

- Visual Studio 2022 (v143 toolset)
- Microsoft Detours kütüphanesi (32-bit)
- Windows SDK
- 32-bit (x86) derleme ortamı

## Proje Yapısı

```
SystemHooks/
├── src/                    
│   ├── dllmain.cpp        
│   ├── HookManager.cpp    
│   ├── MacAddressHook.cpp 
│   └── DiscordRPCHook.cpp 
├── include/            
│   ├── IHook.h         
│   ├── HookManager.h   
│   ├── MacAddressHook.h  
│   ├── DiscordRPCHook.h  
│   └── detours.h
├── lib/                  
│   └── detours.lib     
├── SystemHooks.vcxproj 
├── SystemHooks.sln     
├── .gitignore         
└── README.md

Derleme sonrası:
build/
├── Debug/            
│   └── SystemHooks.dll
└── Release/        
    └── SystemHooks.dll
```

## Kurulum

1. Microsoft Detours kütüphanesini indirin:
   - https://github.com/microsoft/Detours/releases adresinden son sürümü indirin
   - İndirdiğiniz paketi açın

2. Gerekli dosyaları kopyalayın:
   - `include/detours.h` → projedeki `include/` klasörüne
   - `lib.X86/detours.lib` → projedeki `lib/` klasörüne

## Derleme

1. Solution dosyasını açın
2. Platform olarak x86 seçin (önemli: sadece 32-bit desteklenir)
3. Release konfigürasyonunu seçin
4. Projeyi derleyin

## Kullanım

1. 32-bit hedef programı başlatın
2. DLL'i inject etmek için aşağıdaki yöntemlerden birini kullanın:
   - LoadLibrary injection
   - Manual mapping
   - Thread hijacking
   - Diğer internal DLL injection yöntemleri
3. DLL başarıyla yüklendiğinde, tüm hook'lar otomatik olarak aktif olur

## Yeni Hook Ekleme

1. `IHook` arayüzünü implement eden yeni bir sınıf oluşturun
2. Header dosyasını `include` klasörüne ekleyin
3. Kaynak dosyayı `src` klasörüne ekleyin
4. `dllmain.cpp`'de hook'u HookManager'a kaydedin:
```cpp
hookManager.RegisterHook(std::unique_ptr<IHook>(new YeniHook()));
```

## Teknik Detaylar

- DLL sadece 32-bit process'lerde çalışır
- Internal hook kullanıldığı için process içinde çalışır
- Hook'lar merkezi olarak yönetilir
- Thread-safe tasarım
- Modüler ve genişletilebilir yapı

## Güvenlik Notu

Bu DLL sadece test ve eğitim amaçlı geliştirilmiştir. Kötü amaçlı kullanımlardan kaçınılmalıdır. Internal DLL injection işlemi hedef programın çalışmasını etkileyebilir. 