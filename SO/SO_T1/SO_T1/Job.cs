using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace SO_T1
{
    class Job
    {
        private const int ready = 0;
        private const int blocked = 1;
        private const int finished = 2;

        // Editavel pelo usuario
        public string programName; // programa que será executado pelo job
        public int memory; // quantidade de memória necessária;
        public string input_path; // dispositivos de entrada(onde estão os dados);
        public int read_delay; // tempo de acesso a cada dado de leitura
        public string output_path; // arquivos de saída(onde serão colocados os dados e tempo de gravação de cada dado);
        public int write_delay; // tempo de acesso a cada dado de escrita
        public int quantum; // valor do quantum deste processo

        // Sistema
        public bool isInitialized = false;
        public int jobStatus = ready; // status do programa
        public int launchDate; // data de lançamento (um inteiro, de acordo com o timer);
        public int finishDate; // data em que o processo foi concluido
        public float priority = 0.5f; // prioridade (a ser usado de acordo com o escalonador);
        public int timeSpent; // tempo que a CPU gastou executando esse processo
        public int blockCount; // quantas vezes foi bloqueado
        public int calledCount; // quantas vezes foi escalonado
        public int exceedTimeCount; // número de vezes que perdeu a CPU por preempção
        public int timeSpentBlocked; // tempo bloqueado
        public int lastExecutionTime; // tempo da ultima execucao

        // CPU
        public Status cpu_status; // status da cpu
        public string[] programMemory; // memoria de programa
        public int[] dataMemory; // memoria de dados

        public bool IsReady()
        {
            return jobStatus == 0 ? true : false;
        }

        public void Init()
        {
            if (File.Exists(input_path)) // confere se o diretorio existe e le o seu conteudo
            {
                string content = File.ReadAllText(input_path);
                string[] array = content.Split("\n");
                programMemory = array; // manda as instrucoes para a cpu
            }
            else
            {
                Console.WriteLine("Path " + input_path + " was not found.");
            }

            jobStatus = ready;
            timeSpent = 0;
            blockCount = 0;
            calledCount = 0;
            exceedTimeCount = 0;
            timeSpentBlocked = 0;
            lastExecutionTime = 0;
            launchDate = Timer.GetCurrentTime();
            priority = 0.5f;
            cpu_status = new Status();
            dataMemory = new int[memory];
            isInitialized = true;
            Timer.quantumMaxValue = quantum;

            Console.WriteLine("\n\n-------- Initializing job: " + programName + " --------\n\n");
        }

        public void UpdateJobPriority()
        {
            priority = (priority + (Timer.currentQuantum / quantum)) / 2;
            Console.WriteLine("\n" + JobManager.GetCurrentJob().programName + " New priority: " + priority + "\n\n");
        }

        public void UpdateTimeSpent(int time)
        {
            //Console.Write("\nUpdating with time: " + time + "\n\n");
            timeSpent += time;
        }

        public void UpdateJobCPUStatus(Status e) // salvar os dados do Job

        {
            cpu_status = e;
            programMemory = CPU.programMemory;
            dataMemory = CPU.dataMemory;
        }

        public void PutJobOnCPU() // colocar os dados do job na CPU
        {
            cpu_status.InterruptionCode = 0; // set to normal again
            CPU.UpdateCPUStatus(cpu_status); // altera o estado da cpu
            CPU.SetCPUDataMemory(dataMemory); // envia os dados para a cpu
            CPU.SetCPUProgramMemory(programMemory); // envia os dados para a cpu
            Timer.quantumMaxValue = quantum;
            Timer.RestartQuantum(); // reinicia o contador de quantum
            calledCount++; // incrementa o numero de vezes que foi escalonado

            Console.WriteLine("\n\n-------- Continuing job: " + programName + " --------\n\n");
        }

        private void FinishJob()
        {
            finishDate = Timer.currentTime;
            jobStatus = finished;
            JobManager.RemoveJob(this);
        }

        public void UpdateJobStatus(int state)
        {
            if (state == finished && jobStatus != finished) // remove o job finalizado da lista
            {
                FinishJob();
            }

            if(state == ready && jobStatus == blocked) // estava bloqueado e nao estara mais
            {
                timeSpentBlocked += (Timer.currentTime - lastExecutionTime); // atualiza o tempo que passou bloqueado
            }
            else
            if(state == blocked && jobStatus != blocked) // incrementa o contador de bloqueios
            { 
                blockCount++;
                lastExecutionTime = Timer.currentTime;
                UpdateJobPriority(); // atualiza a prioridade do processo
            }

            jobStatus = state;
        }
    }
}
