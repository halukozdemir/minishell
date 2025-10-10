# 🐚 Minishell

**Custom Shell Implementation** - Ecole42 projesi kapsamında geliştirilmiş, bash benzeri komut satırı arayüzü ve shell programlama tekniklerini öğreten kapsamlı bir C projesi.

## 📖 Proje Hakkında / About the Project

Minishell, bash shell'inin temel özelliklerini içeren özel bir shell implementasyonudur. Bu proje, sistem programlama, process yönetimi, pipe işlemleri ve komut parsing gibi kritik sistem programlama kavramlarını öğretir.

Minishell is a custom shell implementation that includes the basic features of bash shell. This project teaches critical system programming concepts such as system programming, process management, pipe operations, and command parsing.

### 🎯 Proje Amacı / Project Purpose

- **Shell Programming**: Komut satırı arayüzü geliştirme / Command line interface development
- **Process Management**: Process oluşturma ve yönetimi / Process creation and management
- **Pipe Implementation**: Processler arası iletişim / Inter-process communication
- **Command Parsing**: Komut satırı parsing ve tokenization / Command line parsing and tokenization
- **Built-in Commands**: Shell built-in komutları implementasyonu / Shell built-in command implementation

## 🏗️ Proje Yapısı / Project Structure

```
minishell/
├── lib/                    # Kütüphaneler / Libraries
│   └── libft/             # 42 libft kütüphanesi / 42 libft library
├── builtins/              # Built-in komutlar / Built-in commands
│   ├── cd.c               # cd komutu / cd command
│   ├── echo.c             # echo komutu / echo command
│   ├── env.c              # env komutu / env command
│   ├── exit.c             # exit komutu / exit command
│   ├── export.c           # export komutu / export command
│   ├── pwd.c              # pwd komutu / pwd command
│   └── unset.c            # unset komutu / unset command
├── executer/              # Komut çalıştırma / Command execution
│   ├── exec.c             # Ana execution logic / Main execution logic
│   ├── pipe.c             # Pipe işlemleri / Pipe operations
│   └── redirection.c      # I/O redirection
├── parser/                # Komut parsing / Command parsing
│   ├── lexer.c            # Tokenization
│   ├── parser.c           # Syntax parsing
│   └── expander.c         # Variable expansion
├── main.c                 # Ana program / Main program
└── minishell.h            # Header dosyası / Header file
```

## 🚀 Kullanım / Usage

### Derleme / Compilation
```bash
make
```

### Çalıştırma / Execution
```bash
./minishell
```

### Örnek Kullanım / Example Usage
```bash
# Temel komutlar / Basic commands
$ ls -la
$ pwd
$ echo "Hello World"

# Pipe işlemleri / Pipe operations
$ ls | grep .c
$ cat file.txt | wc -l

# Redirection
$ echo "test" > file.txt
$ cat < file.txt
```

## 🔧 Built-in Komutlar / Built-in Commands

| Komut / Command | Açıklama / Description | Örnek / Example |
|-----------------|----------------------|-----------------|
| `cd` | Dizin değiştirme / Change directory | `cd /home/user` |
| `echo` | Metin yazdırma / Print text | `echo "Hello"` |
| `env` | Environment variables | `env` |
| `exit` | Shell'den çıkış / Exit shell | `exit` |
| `export` | Variable export | `export VAR=value` |
| `pwd` | Mevcut dizin / Current directory | `pwd` |
| `unset` | Variable silme / Delete variable | `unset VAR` |

## 🔧 Teknik Özellikler / Technical Features

### Process Management
- **Fork/Exec**: Process oluşturma ve çalıştırma / Process creation and execution
- **Wait**: Process bekleme / Process waiting
- **Signal Handling**: Signal yönetimi / Signal management
- **Process Groups**: Process grupları / Process groups

### Pipe Implementation
- **Pipe Creation**: Pipe oluşturma / Pipe creation
- **Pipe Chaining**: Çoklu pipe işlemleri / Multiple pipe operations
- **I/O Redirection**: Input/Output yönlendirme / Input/Output redirection
- **File Descriptor Management**: FD yönetimi / FD management

### Command Parsing
- **Lexical Analysis**: Tokenization
- **Syntax Parsing**: Syntax analizi / Syntax analysis
- **Variable Expansion**: Environment variable genişletme / Environment variable expansion
- **Quote Handling**: Tırnak işleme / Quote processing

### Memory Management
- **Dynamic Allocation**: Dinamik bellek yönetimi / Dynamic memory management
- **Memory Leak Prevention**: Bellek sızıntısı önleme / Memory leak prevention
- **String Management**: String işlemleri / String operations
- **Cleanup Functions**: Temizlik fonksiyonları / Cleanup functions

## 🎨 Özellikler / Features

- ✅ **Command Execution**: Dış komut çalıştırma / External command execution
- ✅ **Pipe Support**: Pipe işlemleri / Pipe operations
- ✅ **Redirection**: I/O redirection
- ✅ **Built-in Commands**: Shell built-in komutları / Shell built-in commands
- ✅ **Environment Variables**: Environment variable yönetimi / Environment variable management
- ✅ **Signal Handling**: Signal yönetimi / Signal management
- ✅ **Error Handling**: Kapsamlı hata kontrolü / Comprehensive error control

## 🔍 Test Senaryoları / Test Scenarios

### Temel Komutlar / Basic Commands
```bash
# Built-in komutlar / Built-in commands
$ pwd
$ echo "test"
$ cd /tmp
$ env

# Dış komutlar / External commands
$ ls -la
$ cat /etc/passwd
$ ps aux
```

### Pipe İşlemleri / Pipe Operations
```bash
# Basit pipe / Simple pipe
$ ls | grep .c

# Çoklu pipe / Multiple pipe
$ ls -la | grep .c | wc -l

# Pipe ve redirection / Pipe and redirection
$ ls | grep .c > files.txt
```

### Redirection
```bash
# Output redirection
$ echo "test" > file.txt
$ ls > files.txt

# Input redirection
$ cat < file.txt
$ wc -l < file.txt

# Append redirection
$ echo "new line" >> file.txt
```

## 📊 Performans / Performance

- **Memory Usage**: Efficient memory management
- **Process Creation**: Fast process spawning
- **Pipe Performance**: Optimized pipe operations
- **Command Parsing**: Fast lexical analysis

## 🛠️ Geliştirme Ortamı / Development Environment

- **Dil / Language**: C
- **Kütüphaneler / Libraries**: libft, system calls
- **Derleyici / Compiler**: gcc
- **Platform**: Linux/Unix
- **System Calls**: fork, exec, pipe, dup2

## 📚 Öğrenilen Kavramlar / Learned Concepts

### System Programming
- **Process Management**: Process oluşturma ve yönetimi / Process creation and management
- **System Calls**: Sistem çağrıları / System calls
- **File Descriptors**: Dosya tanımlayıcıları / File descriptors
- **Signal Handling**: Signal yönetimi / Signal management

### Shell Programming
- **Command Parsing**: Komut analizi / Command analysis
- **Pipe Implementation**: Pipe işlemleri / Pipe operations
- **Redirection**: I/O yönlendirme / I/O redirection
- **Built-in Commands**: Shell komutları / Shell commands

### Memory Management
- **Dynamic Allocation**: Dinamik bellek / Dynamic memory
- **String Handling**: String işlemleri / String operations
- **Memory Leaks**: Bellek sızıntısı önleme / Memory leak prevention
- **Cleanup**: Kaynak temizleme / Resource cleanup

## 🔧 Geliştirme Notları / Development Notes

- **Process Tree**: Process ağacı yönetimi / Process tree management
- **Signal Propagation**: Signal yayılımı / Signal propagation
- **Pipe Chaining**: Çoklu pipe işlemleri / Multiple pipe operations
- **Error Recovery**: Hata kurtarma / Error recovery
- **Memory Safety**: Bellek güvenliği / Memory safety

## 📝 Lisans / License

Bu proje Ecole42 eğitim programı kapsamında geliştirilmiştir.

This project was developed within the scope of the Ecole42 education program.

## 👨‍💻 Geliştirici / Developer

**Haluk Özdemir**  
42 Istanbul - System Programming
**Betül Yıldız**
42 Istanbul - System Programming

---

*Bu proje, shell programlama ve sistem programlama temellerini öğrenmek için tasarlanmıştır. Bash shell'inin temel özelliklerini modern C programlama teknikleri ile implement eder ve gerçek dünya shell programlama deneyimi kazandırır.*

*This project is designed to learn shell programming and system programming fundamentals. It implements the basic features of bash shell with modern C programming techniques and provides real-world shell programming experience.*
