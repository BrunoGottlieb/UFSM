using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SO_T1
{
    class Status
    {
        public int PC { get; set; } // registrador | o contador de programa
        public int A { get; set; } // registrador | acumulador
        public int InterruptionCode { get; set; } // codigo de interrupcao // 0 normal | 1 instrucao ilegal | 2 violacao de memoria
    }

    class CPU
    {
        public const int normal = 0;
        public const int ilegal = 1;
        public const int violacao = 2;
        public const int sleeping = 3;

        public static Status status = new Status(); // estado da CPU

        public static string[] programMemory; // memoria de programa
        public static int[] dataMemory; // memoria de dados

        #region API

        public static int value = 0;

        // altera o valor do acumulador
        public static void SetCPU_A(int newValue)
        {
            status.A = newValue;
        }
        // retorna o valor do acumulador
        public static int GetCPU_A(Status e)
        {
            return e.A;
        }

        // alterar o conteúdo da memória de programa (recebe um vetor de strings)
        public static void SetCPUProgramMemory(string[] newData)
        {
            programMemory = newData;
        }

        // alterar o conteúdo da memória de dados (recebe um vetor de inteiros, que é alterado pela execução das instruções)
        public static void SetCPUDataMemory(int[] newData)
        {
            dataMemory = newData;
        }

        // obter o conteúdo da memória de dados (retorna um vetor de inteiros que é o conteúdo atual da memória – não precisa desta função caso o vetor passado pela função acima seja alterado “in loco”)
        public static int[] GetCPUDataMemory()
        {
            return dataMemory;
        }

        // ler o modo de interrupção da CPU (normal ou um motivo de interrupção)
        public static int GetCPUInterruptionCode()
        {
            return status.InterruptionCode;
        }

        // colocar a CPU em modo normal (coresponde ao retorno de interrupção) – muda para modo normal e incrementa o PC; se já estiver em modo normal, não faz nada
        public static int GetCPUInterruptionMode()
        {
            return status.InterruptionCode;
        }

        // obter a instrução em PC (que pode ser inválida se PC estiver fora da memória de programa)
        public static string GetPCInstruction()
        {
            return programMemory[status.PC];
        }

        // obter o estado interno da CPU (retorna o valor de todos os registradores)
        public static Status GetCPUStatus()
        {
            return status;
        }

        // alterar o estado interno da CPU (copia para os registradores da cpu os valores recebidos)
        public static void UpdateCPUStatus(Status e)
        {
            status = e;
        }

        // inicializar o estado interno da CPU (PC=0, A=0, estado=normal)
        public static void InitializeCPU()
        {
            status.PC = 0;
            status.A = 0;
            status.InterruptionCode = normal; // normal
        }

        public static int GetMemoryDataSize()
        {
            return dataMemory.Length;
        }

        public static void UpdatePC()
        {
            status.PC++;
        }

        // executar uma instrução (só executa se estiver em modo normal)
        public static void ExecuteCPU()
        {
            if (GetCPUInterruptionMode() == normal)
            {
                //Console.WriteLine("Executing");

                bool updatePC = true; // controla se o PC devera ser atualizado ao terminar a execucao da instrucao

                Status status = GetCPUStatus();

                string origem = GetPCInstruction(); // retorna a instrucao atual do PC

                string[] instrucao = origem.Split(' ');
                string instruction = instrucao[0];

                string valueTmp = origem.Remove(0, origem.IndexOf(' ') + 1);


                string valueString = string.Join("", valueTmp.ToCharArray().Where(Char.IsDigit));

                if (valueString.Length > 0)
                    value = Convert.ToInt32(valueString);

                // DEBUG
                {
                    Console.WriteLine("\ninstruction: " + instruction + " " +  value);
                }

                if (instruction == "CARGI") // coloca o valor n no acumulador (A=n)
                {
                    status.A = value;
                }

                else if (instruction == "CARGM") // coloca no acumulador o valor na posição n da memória de dados (A=M[n])
                {
                    int[] data = GetCPUDataMemory();
                    if (value < GetMemoryDataSize())
                    {
                        status.A = data[value];
                    }
                    else
                    {
                        status.InterruptionCode = violacao;
                    }
                }

                else if (instruction == "CARGX") // coloca no acumulador o valor na posição que está na posição n da memória de dados (A=M[M[n]])
                {
                    int pos = 0;
                    if (value < GetMemoryDataSize())
                    {
                        pos = dataMemory[value];
                        status.A = pos;
                    }
                    else
                    {
                        status.InterruptionCode = violacao;
                    }
                }

                else if (instruction == "ARMM") // coloca o valor do acumulador na posição n da memória de dados (M[n]=A)
                {
                    if (value < GetMemoryDataSize())
                    {
                        dataMemory[value] = GetCPU_A(status);
                        SetCPUDataMemory(dataMemory);
                    }
                    else
                    {
                        status.InterruptionCode = violacao;
                    }
                }

                else if (instruction == "ARMX") // 	coloca o valor do acumulador posição que está na posição n da memória de dados (M[M[n]]=A)
                {
                    if (value < GetMemoryDataSize())
                    {
                        int pos = dataMemory[value];
                        dataMemory[pos] = GetCPU_A(status);
                        SetCPUDataMemory(dataMemory);
                    }
                    else
                    {
                        status.InterruptionCode = violacao;
                    }
                }

                else if (instruction == "SOMA") // 	soma ao acumulador o valor no endereço n da memória de dados (A=A+M[n])
                {
                    if (value < GetMemoryDataSize())
                    {
                        status.A = status.A + dataMemory[value];
                    }
                    else
                    {
                        status.InterruptionCode = violacao;
                    }
                }

                else if (instruction == "NEG") // 	inverte o sinal do acumulador (A=-A)
                {
                    status.A *= -1;
                }

                else if (instruction == "DESVZ") // se A vale 0, coloca o valor n no PC
                {
                    if (status.A == 0)
                    {
                        status.PC = value;
                        updatePC = false;
                    }
                }

                else // coloca a CPU em interrupção – instrução ilegal
                {
                    status.InterruptionCode = ilegal;
                    updatePC = false;
                }

                UpdateCPUStatus(status); // atualiza o estado da CPU com os novos dados

                if (updatePC) { UpdatePC(); }

            }
        }

        #endregion

    }
}
