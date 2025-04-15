#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include <mutex>
#include <algorithm>
#include "banco.h"

using namespace std;

vector<Registro> bancoDeDados;
mutex bancoMutex;

void salvarBancoEmArquivo(const string& nomeArquivo) {
    ofstream arquivo(nomeArquivo);
    for (const auto& r : bancoDeDados) {
        arquivo << r.id << ";" << r.nome << endl;
    }
}

void carregarBancoDoArquivo(const string& nomeArquivo) {
    bancoDeDados.clear();
    ifstream arquivo(nomeArquivo);
    string linha;
    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        string idStr, nome;
        if (getline(ss, idStr, ';') && getline(ss, nome)) {
            Registro r;
            r.id = stoi(idStr);
            strncpy(r.nome, nome.c_str(), sizeof(r.nome) - 1);
            r.nome[sizeof(r.nome) - 1] = '\0';
            bancoDeDados.push_back(r);
        }
    }
}

string processaRequisicao(const string& req) {
    lock_guard<mutex> lock(bancoMutex);
    stringstream resposta;

    if (req.find("INSERT") == 0) {
        Registro r;
        r.id = bancoDeDados.empty() ? 1 : bancoDeDados.back().id + 1;
        strncpy(r.nome, req.substr(7).c_str(), sizeof(r.nome) - 1);
        r.nome[sizeof(r.nome) - 1] = '\0';
        bancoDeDados.push_back(r);
        salvarBancoEmArquivo("banco.txt");
        resposta << "Registro inserido com ID: " << r.id;

    } else if (req.find("SELECT") == 0) {
        string arg = (req.length() > 7) ? req.substr(7) : "";
        if (!arg.empty() && arg[0] == ' ') arg = arg.substr(1);

        if (arg == "*") {
            for (const auto& r : bancoDeDados) {
                resposta << "ID: " << r.id << " | Nome: " << r.nome << "\n";
            }
        } else {
            try {
                int id = stoi(arg);
                auto it = find_if(bancoDeDados.begin(), bancoDeDados.end(), [id](const Registro& r) {
                    return r.id == id;
                });

                if (it != bancoDeDados.end()) {
                    resposta << "ID: " << it->id << " | Nome: " << it->nome;
                } else {
                    resposta << "ID " << id << " não encontrado.";
                }
            } catch (...) {
                resposta << "Comando SELECT inválido. Use SELECT * ou SELECT <id>";
            }
        }

    } else if (req.find("DELETE") == 0) {
        int id = stoi(req.substr(7));
        auto it = find_if(bancoDeDados.begin(), bancoDeDados.end(), [id](const Registro& r) {
            return r.id == id;
        });

        if (it != bancoDeDados.end()) {
            bancoDeDados.erase(it);
            salvarBancoEmArquivo("banco.txt");
            resposta << "Registro com ID " << id << " removido.";
        } else {
            resposta << "ID inválido.";
        }

    } else if (req.find("UPDATE") == 0) {
        stringstream ss(req.substr(7));
        int id;
        string novoNome;
        ss >> id;
        getline(ss, novoNome);
        novoNome = novoNome.substr(1); // Remove espaço após o ID

        auto it = find_if(bancoDeDados.begin(), bancoDeDados.end(), [id](const Registro& r) {
            return r.id == id;
        });

        if (it != bancoDeDados.end()) {
            strncpy(it->nome, novoNome.c_str(), sizeof(it->nome) - 1);
            it->nome[sizeof(it->nome) - 1] = '\0';
            salvarBancoEmArquivo("banco.txt");
            resposta << "Registro com ID " << id << " atualizado para: " << it->nome;
        } else {
            resposta << "ID não encontrado.";
        }

    } else {
        resposta << "Comando inválido.";
    }

    return resposta.str();
}

void atenderCliente(HANDLE Handle_Pipe) {
    char buffer[512];
    DWORD Bytes_Lidos;
    DWORD threadId = GetCurrentThreadId(); // ID da thread atual

    while (true) {
        BOOL sucesso = ReadFile(Handle_Pipe, buffer, sizeof(buffer) - 1, &Bytes_Lidos, NULL);
        if (!sucesso || Bytes_Lidos == 0) {
            cout << "Cliente desconectado.\n";
            break;
        }

        buffer[Bytes_Lidos] = '\0';
        string comando(buffer);

        if (comando == "SAIR") {
            cout << "Cliente pediu para sair.\n";
            break;
        }

        string resposta = processaRequisicao(comando);

        // Adiciona o ID da thread à resposta
        stringstream respostaComThread;
        respostaComThread << "[Thread ID: " << threadId << "] " << resposta;

        DWORD Bytes_Escritos;
        WriteFile(Handle_Pipe, respostaComThread.str().c_str(), respostaComThread.str().size(), &Bytes_Escritos, NULL);
    }

    FlushFileBuffers(Handle_Pipe);
    DisconnectNamedPipe(Handle_Pipe);
    CloseHandle(Handle_Pipe);
}


int main() {
    carregarBancoDoArquivo("banco.txt");

    cout << "Servidor aguardando conexões... teste123" << endl;

    while (true) {
        HANDLE Handle_Pipe = CreateNamedPipe(
            TEXT("\\\\.\\pipe\\MeuPipe"),
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            512, 512, 0, NULL);

        if (Handle_Pipe == INVALID_HANDLE_VALUE) {
            cerr << "Erro ao criar o pipe." << endl;
            return 1;
        }

        BOOL conectado = ConnectNamedPipe(Handle_Pipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
        if (conectado) {
            cout << "Cliente conectado!" << endl;
            thread t(atenderCliente, Handle_Pipe);
            t.detach();
        }
    }

    return 0;
}
