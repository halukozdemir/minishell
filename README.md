# 🐚 Minishell

**Custom Shell Implementation** - Ecole42 projesi kapsamında geliştirilmiş, bash benzeri komut satırı arayüzü ve shell programlama tekniklerini öğreten kapsamlı bir C projesi.

## 📖 Proje Hakkında

Minishell, bash shell'inin temel özelliklerini içeren özel bir shell implementasyonudur. Bu proje, sistem programlama, process yönetimi, pipe işlemleri ve komut parsing gibi kritik sistem programlama kavramlarını öğretir.

### 🎯 Proje Amacı

- **Shell Programming**: Komut satırı arayüzü geliştirme
- **Process Management**: Process oluşturma ve yönetimi
- **Pipe Implementation**: Processler arası iletişim
- **Command Parsing**: Komut satırı parsing ve tokenization
- **Built-in Commands**: Shell built-in komutları implementasyonu

## 🏗️ Proje Yapısı

```
minishell/
├── lib/                    # Kütüphaneler
│   └── libft/             # 42 libft kütüphanesi
├── builtins/              # Built-in komutlar
│   ├── cd.c               # cd komutu
│   ├── ctrl_builtins.c    # Built-in kontrol fonksiyonları
│   ├── ctrl_builtins_utils.c # Built-in yardımcı fonksiyonlar
│   ├── ctrl_builtins_utils2.c # Ek built-in yardımcı fonksiyonlar
│   ├── echo.c             # echo komutu
│   ├── env.c              # env komutu
│   ├── exit.c             # exit komutu
│   ├── exit_utils.c       # exit yardımcı fonksiyonları
│   ├── export.c            # export komutu
│   ├── export_utils.c     # export yardımcı fonksiyonları
│   ├── pwd.c              # pwd komutu
│   └── unset.c            # unset komutu
├── executer/              # Komut çalıştırma
│   ├── command.c          # Komut işleme
│   ├── environment.c      # Environment variable yönetimi
│   ├── executor.c         # Ana execution logic
│   ├── executor_utils.c   # Execution yardımcı fonksiyonları
│   ├── executor_utils_2.c # Ek execution yardımcı fonksiyonları
│   ├── exec_error.c       # Execution hata yönetimi
│   ├── exec_error_2.c     # Ek execution hata yönetimi
│   ├── heredoc.c          # Here document işleme
│   ├── pipe.c             # Pipe işlemleri
│   ├── redirection.c      # I/O redirection
│   ├── redirection_2.c    # Ek redirection fonksiyonları
│   ├── run_env.c          # Environment çalıştırma
│   └── signals.c          # Signal yönetimi
├── parser/                # Komut parsing
│   ├── dollar.c           # $ değişken genişletme
│   ├── dollar_utils_2.c   # $ yardımcı fonksiyonları
│   ├── job_utils.c        # Job yönetimi yardımcı fonksiyonları
│   ├── parser.c           # Ana parser
│   ├── parser_utils.c     # Parser yardımcı fonksiyonları
│   ├── parser_utils_2.c   # Ek parser yardımcı fonksiyonları
│   ├── redir_utils.c      # Redirection yardımcı fonksiyonları
│   ├── shell_utils.c      # Shell yardımcı fonksiyonları
│   └── syntax_utils.c     # Syntax kontrol yardımcı fonksiyonları
├── main.c                 # Ana program
├── minishell.h            # Header dosyası
└── Makefile               # Derleme kuralları
```

## 🚀 Kullanım

### Derleme
```bash
make
```

### Çalıştırma
```bash
./minishell
```

### Örnek Kullanım
```bash
# Temel komutlar
$ ls -la
$ pwd
$ echo "Hello World"

# Pipe işlemleri
$ ls | grep .c
$ cat file.txt | wc -l

# Redirection
$ echo "test" > file.txt
$ cat < file.txt
```

## 🔧 Built-in Komutlar

| Komut | Açıklama | Örnek |
|-------|----------|-------|
| `cd` | Dizin değiştirme | `cd /home/user` |
| `echo` | Metin yazdırma | `echo "Hello"` |
| `env` | Environment variables | `env` |
| `exit` | Shell'den çıkış | `exit` |
| `export` | Variable export | `export VAR=value` |
| `pwd` | Mevcut dizin | `pwd` |
| `unset` | Variable silme | `unset VAR` |

## 🔧 Teknik Özellikler

### Process Management
- **Fork/Exec**: Process oluşturma ve çalıştırma
- **Wait**: Process bekleme
- **Signal Handling**: Signal yönetimi
- **Process Groups**: Process grupları

### Pipe Implementation
- **Pipe Creation**: Pipe oluşturma
- **Pipe Chaining**: Çoklu pipe işlemleri
- **I/O Redirection**: Input/Output yönlendirme
- **File Descriptor Management**: FD yönetimi

### Command Parsing
- **Lexical Analysis**: Tokenization
- **Syntax Parsing**: Syntax analizi
- **Variable Expansion**: Environment variable genişletme
- **Quote Handling**: Tırnak işleme

### Memory Management
- **Dynamic Allocation**: Dinamik bellek yönetimi
- **Memory Leak Prevention**: Bellek sızıntısı önleme
- **String Management**: String işlemleri
- **Cleanup Functions**: Temizlik fonksiyonları

## 🎨 Özellikler

- ✅ **Command Execution**: Dış komut çalıştırma
- ✅ **Pipe Support**: Pipe işlemleri
- ✅ **Redirection**: I/O redirection
- ✅ **Built-in Commands**: Shell built-in komutları
- ✅ **Environment Variables**: Environment variable yönetimi
- ✅ **Signal Handling**: Signal yönetimi
- ✅ **Error Handling**: Kapsamlı hata kontrolü

## 🔍 Test Senaryoları

### Temel Komutlar
```bash
# Built-in komutlar
$ pwd
$ echo "test"
$ cd /tmp
$ env

# Dış komutlar
$ ls -la
$ cat /etc/passwd
$ ps aux
```

### Pipe İşlemleri
```bash
# Basit pipe
$ ls | grep .c

# Çoklu pipe
$ ls -la | grep .c | wc -l

# Pipe ve redirection
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

## 📊 Performans

- **Memory Usage**: Efficient memory management
- **Process Creation**: Fast process spawning
- **Pipe Performance**: Optimized pipe operations
- **Command Parsing**: Fast lexical analysis

## 🛠️ Geliştirme Ortamı

- **Dil**: C
- **Kütüphaneler**: libft, readline, system calls
- **Derleyici**: gcc
- **Platform**: Linux/Unix
- **System Calls**: fork, exec, pipe, dup2

## 📚 Öğrenilen Kavramlar

### System Programming
- **Process Management**: Process oluşturma ve yönetimi
- **System Calls**: Sistem çağrıları
- **File Descriptors**: Dosya tanımlayıcıları
- **Signal Handling**: Signal yönetimi

### Shell Programming
- **Command Parsing**: Komut analizi
- **Pipe Implementation**: Pipe işlemleri
- **Redirection**: I/O yönlendirme
- **Built-in Commands**: Shell komutları

### Memory Management
- **Dynamic Allocation**: Dinamik bellek
- **String Handling**: String işlemleri
- **Memory Leaks**: Bellek sızıntısı önleme
- **Cleanup**: Kaynak temizleme

## 🔧 Geliştirme Notları

- **Process Tree**: Process ağacı yönetimi
- **Signal Propagation**: Signal yayılımı
- **Pipe Chaining**: Çoklu pipe işlemleri
- **Error Recovery**: Hata kurtarma
- **Memory Safety**: Bellek güvenliği

## 📝 Lisans

Bu proje Ecole42 eğitim programı kapsamında geliştirilmiştir.

## 👨‍💻 Geliştirici

**Haluk Özdemir**  
42 Istanbul - System Programming

**Betül Yıldız**  
42 Istanbul - System Programming

---

*Bu proje, shell programlama ve sistem programlama temellerini öğrenmek için tasarlanmıştır. Bash shell'inin temel özelliklerini modern C programlama teknikleri ile implement eder ve gerçek dünya shell programlama deneyimi kazandırır.*

---

# 🐚 Minishell

**Custom Shell Implementation** - A comprehensive C project developed within the scope of Ecole42, teaching bash-like command line interface and shell programming techniques.

## 📖 About the Project

Minishell is a custom shell implementation that includes the basic features of bash shell. This project teaches critical system programming concepts such as system programming, process management, pipe operations, and command parsing.

### 🎯 Project Purpose

- **Shell Programming**: Command line interface development
- **Process Management**: Process creation and management
- **Pipe Implementation**: Inter-process communication
- **Command Parsing**: Command line parsing and tokenization
- **Built-in Commands**: Shell built-in command implementation

## 🏗️ Project Structure

```
minishell/
├── lib/                    # Libraries
│   └── libft/             # 42 libft library
├── builtins/              # Built-in commands
│   ├── cd.c               # cd command
│   ├── ctrl_builtins.c    # Built-in control functions
│   ├── ctrl_builtins_utils.c # Built-in helper functions
│   ├── ctrl_builtins_utils2.c # Additional built-in helper functions
│   ├── echo.c             # echo command
│   ├── env.c              # env command
│   ├── exit.c             # exit command
│   ├── exit_utils.c       # exit helper functions
│   ├── export.c           # export command
│   ├── export_utils.c     # export helper functions
│   ├── pwd.c              # pwd command
│   └── unset.c            # unset command
├── executer/              # Command execution
│   ├── command.c          # Command processing
│   ├── environment.c     # Environment variable management
│   ├── executor.c         # Main execution logic
│   ├── executor_utils.c   # Execution helper functions
│   ├── executor_utils_2.c # Additional execution helper functions
│   ├── exec_error.c       # Execution error management
│   ├── exec_error_2.c     # Additional execution error management
│   ├── heredoc.c          # Here document processing
│   ├── pipe.c             # Pipe operations
│   ├── redirection.c      # I/O redirection
│   ├── redirection_2.c    # Additional redirection functions
│   ├── run_env.c          # Environment execution
│   └── signals.c          # Signal management
├── parser/                # Command parsing
│   ├── dollar.c           # $ variable expansion
│   ├── dollar_utils_2.c   # $ helper functions
│   ├── job_utils.c        # Job management helper functions
│   ├── parser.c           # Main parser
│   ├── parser_utils.c     # Parser helper functions
│   ├── parser_utils_2.c   # Additional parser helper functions
│   ├── redir_utils.c      # Redirection helper functions
│   ├── shell_utils.c      # Shell helper functions
│   └── syntax_utils.c     # Syntax control helper functions
├── main.c                 # Main program
├── minishell.h            # Header file
└── Makefile               # Compilation rules
```

## 🚀 Usage

### Compilation
```bash
make
```

### Execution
```bash
./minishell
```

### Example Usage
```bash
# Basic commands
$ ls -la
$ pwd
$ echo "Hello World"

# Pipe operations
$ ls | grep .c
$ cat file.txt | wc -l

# Redirection
$ echo "test" > file.txt
$ cat < file.txt
```

## 🔧 Built-in Commands

| Command | Description | Example |
|---------|-------------|---------|
| `cd` | Change directory | `cd /home/user` |
| `echo` | Print text | `echo "Hello"` |
| `env` | Environment variables | `env` |
| `exit` | Exit shell | `exit` |
| `export` | Variable export | `export VAR=value` |
| `pwd` | Current directory | `pwd` |
| `unset` | Delete variable | `unset VAR` |

## 🔧 Technical Features

### Process Management
- **Fork/Exec**: Process creation and execution
- **Wait**: Process waiting
- **Signal Handling**: Signal management
- **Process Groups**: Process groups

### Pipe Implementation
- **Pipe Creation**: Pipe creation
- **Pipe Chaining**: Multiple pipe operations
- **I/O Redirection**: Input/Output redirection
- **File Descriptor Management**: FD management

### Command Parsing
- **Lexical Analysis**: Tokenization
- **Syntax Parsing**: Syntax analysis
- **Variable Expansion**: Environment variable expansion
- **Quote Handling**: Quote processing

### Memory Management
- **Dynamic Allocation**: Dynamic memory management
- **Memory Leak Prevention**: Memory leak prevention
- **String Management**: String operations
- **Cleanup Functions**: Cleanup functions

## 🎨 Features

- ✅ **Command Execution**: External command execution
- ✅ **Pipe Support**: Pipe operations
- ✅ **Redirection**: I/O redirection
- ✅ **Built-in Commands**: Shell built-in commands
- ✅ **Environment Variables**: Environment variable management
- ✅ **Signal Handling**: Signal management
- ✅ **Error Handling**: Comprehensive error control

## 🔍 Test Scenarios

### Basic Commands
```bash
# Built-in commands
$ pwd
$ echo "test"
$ cd /tmp
$ env

# External commands
$ ls -la
$ cat /etc/passwd
$ ps aux
```

### Pipe Operations
```bash
# Simple pipe
$ ls | grep .c

# Multiple pipe
$ ls -la | grep .c | wc -l

# Pipe and redirection
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

## 📊 Performance

- **Memory Usage**: Efficient memory management
- **Process Creation**: Fast process spawning
- **Pipe Performance**: Optimized pipe operations
- **Command Parsing**: Fast lexical analysis

## 🛠️ Development Environment

- **Language**: C
- **Libraries**: libft, readline, system calls
- **Compiler**: gcc
- **Platform**: Linux/Unix
- **System Calls**: fork, exec, pipe, dup2

## 📚 Learned Concepts

### System Programming
- **Process Management**: Process creation and management
- **System Calls**: System calls
- **File Descriptors**: File descriptors
- **Signal Handling**: Signal management

### Shell Programming
- **Command Parsing**: Command analysis
- **Pipe Implementation**: Pipe operations
- **Redirection**: I/O redirection
- **Built-in Commands**: Shell commands

### Memory Management
- **Dynamic Allocation**: Dynamic memory
- **String Handling**: String operations
- **Memory Leaks**: Memory leak prevention
- **Cleanup**: Resource cleanup

## 🔧 Development Notes

- **Process Tree**: Process tree management
- **Signal Propagation**: Signal propagation
- **Pipe Chaining**: Multiple pipe operations
- **Error Recovery**: Error recovery
- **Memory Safety**: Memory safety

## 📝 License

This project was developed within the scope of the Ecole42 education program.

## 👨‍💻 Developer

**Haluk Özdemir**  
42 Istanbul - System Programming

**Betül Yıldız**  
42 Istanbul - System Programming

---

*This project is designed to learn shell programming and system programming fundamentals. It implements the basic features of bash shell with modern C programming techniques and provides real-world shell programming experience.*