#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <thread>
#include <string>
#include <mutex>

#pragma warning(disable: 4996)

int N = 64;



void check_s(std::string& s);
void sorting(std::string& s);
void replacement(std::string& s);
void A(std::string& s);

int sum(std::string& s);
void B(std::string& s);

std::mutex mtx;

int main(int argc, char* argv[])
{
    std::string s;

    std::thread thr_1(A, std::ref(s));

    std::thread thr_2(B, std::ref(s));

    thr_1.join();
    thr_2.join();

}

void check_s(std::string& s)
{

    bool k = 1;
    do
    {
        std::cout << "Enter numbers (max 64 simbols):" << std::endl;
        std::cin >> s;

        if (s.size() > N)
        {
            s.resize(N);
        }


        for (int i = 0; i < s.size(); i++)
        {
            if (s[i] >= '0' && s[i] <= '9')
            {
                k = 0;
                continue;
            }
            else
            {
                k = 1;
                std::cout << "Error. Enter only numbers." << std::endl;
                break;
            }
        }

    } while (k == 1);
}

void sorting(std::string& s)
{
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == s[0])
        {
            s.insert(0, 1, s[i]);
            s.erase(i + 1, 1);
            continue;
        }
        else if (s[i] > s[0])
        {
            s.insert(0, 1, s[i]);
            s.erase(i + 1, 1);
        }
        else if (s[i] < s[0])
        {
            for (int j = 0; j <= i; j++)
            {
                if (s[i] < s[j])
                {
                    continue;
                }
                else if (s[i] >= s[j])
                {
                    s.insert(j, 1, s[i]);
                    s.erase(i + 1, 1);
                    break;
                }

            }
        }
    }
}

void replacement(std::string& s)
{
    for (int i = 0; i < s.size(); i++)
    {
        if ((s[i] & 1) == 0)
        {
            s.insert(i, "KB");
            s.erase(i + 2, 1);
            ++i;
        }
        else continue;
    }
}

void A(std::string& s)
{
    while (true)
    {
        mtx.lock();
        check_s(s);
        sorting(s);
        replacement(s);

        mtx.unlock();
        Sleep(5);
    }

}

int sum(std::string& s)
{
    int sum = 0;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] >= '0' && s[i] <= '9')
        {
            sum += s[i] - '0';
        }
        else continue;
    }
    return sum;
}

void B(std::string& s)
{
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "Error" << std::endl;
        exit(1);
    }

    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
    listen(sListen, SOMAXCONN);

    SOCKET newConnection;
    newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

    char msg[256];

    while (true)
    {
        Sleep(20);

        mtx.lock();
        std::string s2 = s;
        s.clear();
        mtx.unlock();
        std::cout << s2 << std::endl;
        std::cout << sum(s2) << std::endl;

        std::string s3 = std::to_string(sum(s2));
        
        strcpy(msg, s3.c_str());

        send(newConnection, msg, sizeof(msg), NULL);

    }

}

