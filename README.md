# 🛠️ Sistema Cliente-Servidor com Named Pipes (Windows)

Este projeto é um sistema cliente-servidor desenvolvido em **C++** utilizando **Named Pipes do Windows** para comunicação entre processos. O servidor mantém um pequeno banco de dados em memória com persistência em arquivo, permitindo operações básicas de CRUD.

---

## 📚 Funcionalidades

✅ Inserir novos registros  
✅ Consultar todos os registros ou por ID  
✅ Atualizar o nome de um registro existente  
✅ Remover registros por ID  
✅ Persistência automática dos dados em `banco.txt`  
✅ Multithreading no servidor para múltiplos clientes simultâneos  
✅ Inicialização automática do servidor pelo cliente (se não estiver rodando)

---

## 🧱 Tecnologias Utilizadas

- C++
- Windows API
- Named Pipes
- Multithreading
- I/O com arquivos (`fstream`)

---
## 🚀 Como Executar

### Pré-requisitos

- Compilador C++ com suporte à API do Windows (ex: MSVC)
- Sistema operacional Windows

### Passo a passo

1. Compile os arquivos:
   ```
   g++ Servidor.cpp -o Servidor.exe -lws2_32
   g++ Cliente.cpp -o Cliente.exe

./Cliente.exe

O cliente irá tentar se conectar ao servidor. Se não encontrar, ele inicia o servidor automaticamente.

Comando | Ação
INSERT nome | Adiciona um novo registro com o nome informado
SELECT * | Exibe todos os registros
SELECT id | Exibe o registro com o ID especificado
UPDATE id novo_nome | Atualiza o nome do registro com o ID dado
DELETE id | Remove o registro com o ID especificado
SAIR | Encerra a conexão com o servidor

