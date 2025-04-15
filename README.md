# Sistema Cliente-Servidor com Named Pipes (Windows)

Projeto realizado por: Gustavo Cadore da Silva, Lucas Rodrigues Pinheiro e Pedro Henrique Souza dos Santos.

---

Este projeto é um sistema cliente-servidor desenvolvido em **C++** utilizando **Named Pipes do Windows** para comunicação entre processos. O servidor mantém um pequeno banco de dados em memória com persistência em arquivo, permitindo operações básicas de CRUD.

---

## Funcionalidades

✅ Inserir novos registros  
✅ Consultar todos os registros ou por ID  
✅ Atualizar o nome de um registro existente  
✅ Remover registros por ID  
✅ Persistência automática dos dados em `banco.txt`  
✅ Multithreading no servidor para múltiplos clientes simultâneos  
✅ Inicialização automática do servidor pelo cliente (se não estiver rodando)

---

## Tecnologias Utilizadas

- C++
- Windows API
- Named Pipes
- Multithreading
- I/O com arquivos (`fstream`)

---
## Como Executar

### Pré-requisitos

- Compilador C++ com suporte à API do Windows (ex: MSVC)
- Sistema operacional Windows

### Passo a passo

1. Compile os arquivos:
   ```
   g++ Servidor.cpp -o Servidor.exe
   g++ Cliente.cpp -o Cliente.exe

2. execute o .exe pelo cmd
   ```
   ./Cliente.exe
O cliente irá tentar se conectar ao servidor. Se não encontrar, ele inicia o servidor automaticamente.


## Comandos Disponíveis no Cliente

Abaixo estão os comandos que você pode usar no terminal do cliente para interagir com o servidor:

| Comando                   | Descrição                                                                 |
|---------------------------|---------------------------------------------------------------------------|
| `INSERT nome`             | Insere um novo registro com o nome especificado.                          |
| `SELECT *`                | Exibe todos os registros existentes no banco.                             |
| `SELECT id`               | Exibe o registro com o ID informado.                                      |
| `UPDATE id novo_nome`     | Atualiza o nome do registro com o ID fornecido.                           |
| `DELETE id`               | Remove o registro com o ID indicado.                                      |
| `SAIR`                    | Encerra a conexão com o servidor e fecha o cliente.                       |


