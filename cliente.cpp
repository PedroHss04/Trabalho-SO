#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

bool iniciarServidor() {
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);

    // Caminho para o executável do servidor
    string caminhoServidor = "Servidor.exe";

    // Tenta criar o processo do servidor
   if (!CreateProcessA(
        NULL,
        const_cast<char*>(caminhoServidor.c_str()),
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi)) {
            cerr << "Erro ao iniciar o servidor. Código: " << GetLastError() << endl;
            return false;
        }

    cout << "Servidor iniciado com sucesso. Aguardando conexão...\n";

    Sleep(1000);

    // Fecha os handles do processo
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}

int main() {
    HANDLE Handle_Pipe;

    // Tenta se conectar ao pipe
    Handle_Pipe = CreateFile(
        TEXT("\\\\.\\pipe\\MeuPipe"),
        GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, 0, NULL);

    // Se o pipe ainda não existir, inicia o servidor através de um processo
    if (Handle_Pipe == INVALID_HANDLE_VALUE) {
        cout << "Servidor não está rodando. Tentando iniciar...\n";
        if (iniciarServidor()) {
            // Tenta se conectar novamente
            Handle_Pipe = CreateFile(
                TEXT("\\\\.\\pipe\\MeuPipe"),
                GENERIC_READ | GENERIC_WRITE,
                0, NULL, OPEN_EXISTING, 0, NULL);

            if (Handle_Pipe == INVALID_HANDLE_VALUE) {
                cerr << "Não foi possível conectar ao pipe mesmo após iniciar o servidor.\n";
                return 1;
            }
        } else {
            return 1;
        }
    }

    string Instrucao;

    while (true) {
        cout << "\nDigite uma requisição:\n";
        cout << "  INSERT nome         -> Adiciona novo registro\n";
        cout << "  DELETE id           -> Remove um registro pelo ID\n";
        cout << "  SELECT *             -> Mostra todos os registros\n";
        cout << "  SELECT id           -> Mostra o registro com o ID especificado\n";
        cout << "  UPDATE id novo_nome -> Atualiza o nome do registro\n";
        cout << "  SAIR                -> Encerra o cliente\n";
        cout << ">> ";

        getline(cin, Instrucao);

        if (Instrucao == "SAIR") {
            DWORD Bytes_Escritos;
            WriteFile(Handle_Pipe, Instrucao.c_str(), Instrucao.size(), &Bytes_Escritos, NULL);
            break;
        }

        DWORD Bytes_Escritos; // Comunicação com o pipe do servidor
        WriteFile(Handle_Pipe, Instrucao.c_str(), Instrucao.size(), &Bytes_Escritos, NULL);

        char respostaServidor[512];
        DWORD Bytes_Lidos;

        if (ReadFile(Handle_Pipe, respostaServidor, sizeof(respostaServidor) - 1, &Bytes_Lidos, NULL)) {
            respostaServidor[Bytes_Lidos] = '\0';
            cout << "\nResposta do servidor:\n" << respostaServidor << endl;
        } else {
            cerr << "Erro ao ler do pipe." << endl;
        }
    }

    CloseHandle(Handle_Pipe);
    return 0;
}
