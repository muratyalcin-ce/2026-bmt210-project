
# Gazi Üniversitesi 2026 Yılı BMT210 (Veri Yapıları) Dersi Projesi
Hazırlayanlar:

- Murat Yalçın (24181616002)
- Ömer Faruk Altun (24181616045)

# Genel Bilgiler
C programlama dili ve [Raylib](https://www.raylib.com/) kütüphanesi kullanılarak geliştirilmiş, katmanlı tuval, geri alma/yineleme geçmişi ve çoklu çizim araçlarına sahip Paint benzeri bir çizim uygulaması.

---

# Release
Hazır derlenmiş çalıştırılabilirler.
(bkz. [Releases](https://github.com/muratyalcin-ce/2026-bmt210-project/releases))

# Kaynak Koddan Derleme
## Gereksinimler

Windows için Visual Studio Build Tools kullanılması önerilir.

- **Make/Ninja** 
- **CMake** 3.16+
- **C derleyici** —-GCC veya Clang (Windows'ta MSVC)
- **Git**

Raylib bu depoya alt modül olarak eklenmiştir, ayrıca kurulum gerekmez.

---

## Derleme


```bash
# 1. Depoyu alt modüllerle birlikte klonla
git clone --recurse-submodules https://github.com/muratyalcin-ce/2026-bmt210-project.git
cd 2026-bmt210-project

# 2. Yapılandır (Linux)
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 2. Yapılandır (Windows)
cmake -B build -DCMAKE_BUILD_TYPE=Release -G Ninja

# 3. Derle
cmake --build build --config Release
```

> `--recurse-submodules` olmadan klonladıysanız şunu çalıştırın:
> ```bash
> git submodule update --init --recursive
> ```

---

## Çalıştırma

```bash
# Linux / macOS
./build/MyDraw

# Windows
.\build\Release\MyDraw.exe
```

---

## Kontroller
Kontroller arayüzde bulunan butonlar aracılığıyla da kullanılabilir.

| İşlem | Kısayol |
|---|---|
| Çizim | Fırça(`B`) + Sol Fare Tuşu (basılı tut) |
| Silgi | Silgi aracını seç(`E`) + Sol Fare Tuşu |
| Geri Al | `Z` |
| Yinele |  `Y` |
| Katman görünürlüğü | `V` |

---

## Proje Yapısı

```
2026-bmt210-project/
├── CMakeLists.txt
├── src/                         # Kaynak dosyalar (.c)
├── include/                     # Başlık dosyaları (.h)
├── external/
│   └── raylib/                  # Raylib alt modülü
└── README.md
```

---

## Lisans

MIT (bkz. [LICENSE](LICENSE) )
