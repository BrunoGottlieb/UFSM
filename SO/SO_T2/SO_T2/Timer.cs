using System;
using System.Collections.Generic;
using System.Text;

namespace SO_T2
{
    class Schedule
    {
        public Job job { get; set; } // job que causou essa interrupcao
        public char type { get; set; } // tipo de agendamento de interrupcao
        public int date { get; set; } // quando
        public int period { get; set; } // tempo original
        public int interruptionCode { get; set; } // que interrupcao sera gerada
        public int frame { get; set; } // usado apenas para operacoes de memoria secundaria
    }

    class Timer
    {
        private static List<Schedule> queue = new List<Schedule>();

        public static int currentTime { get; set; } // contador de tempo

        public static int currentQuantum { get; set; } // quantum do processo em execucao

        public static int quantumMaxValue { get; set; } // valor de cada quantum

        #region Quantum
        public static void RestartQuantum()
        {
            currentQuantum = 0;
        }

        public static void UpdateQuantum()
        {
            currentQuantum++;
            CheckQuantum();
        }

        private static void CheckQuantum()
        {
            if (currentQuantum >= quantumMaxValue && CPU.GetCPUInterruptionCode() != 3) // caso o tempo de quantum tenha atingido seu limite
            {
                Console.WriteLine("\n*** MAX QUANTUM ***\n");
                Job currentJob = JobManager.GetCurrentJob();
                currentJob.UpdateJobPriority(); // atualiza a prioridade pois o quantum acabou
                SO.TimerCallBack(currentJob, 0, -1); // gera uma interrupcao
            }
        }

        #endregion

        public static void Initialize()
        {
            currentTime = 0;
            currentQuantum = 0;
            queue.Clear();
        }

        // informar a passagem do tempo (ele simplesmente incrementa um contador interno)
        public static int UpdateTime()
        {
            currentTime++;
            if (CPU.GetCPUInterruptionCode() == 0) { UpdateQuantum(); }// incrementa tambem o contador de quantum atual
            Console.WriteLine("Timer current time: " + currentTime);
            return GetInterruption(); // checa se ele esta causando alguma interrupcao
        }

        // ler o tempo atual (retorna o valor atual do contador)
        public static int GetCurrentTime()
        {
            return currentTime;
        }

        // verificar se tem uma interrupção pendente - ele retorna o código da interrupção ou um código para dizer que não tem nenhuma interrupção.
        // Essa função pode ser chamada diversas vezes, para se saber se tem várias interrupções no mesmo tempo – o timer “esquece” cada interrupção que ele retorna
        public static int GetInterruption()
        {
            if (queue.Count == 0) { return 0; } // nao ha interrupcoes
            if (currentTime < queue[0].date) { return 0; } // ainda nao chegou a interrupcao
            else
            {
                Schedule schedule = new Schedule();
                schedule = queue[0]; // pega a interrupcao
                queue.RemoveAt(0); // remove da fila
                if (schedule.type == 'P')
                {
                    NewInterruption(schedule.job, 'P', schedule.period + currentTime, schedule.interruptionCode, schedule.frame); // reinsere as periodicas
                }
                SO.TimerCallBack(schedule.job, 0, schedule.frame);
                return schedule.interruptionCode; // retorna o codigo da interrupcao
            }
        }

        // gerar novas interrupções – deve informar o tipo (periódica ou não), o período (tempo entre interrupções) ou tempo até a interrupção, e o código da interrupção que será gerada
        public static void NewInterruption(Job j, char type, int date, int interruptionCode, int frame)
        {
            Schedule newSchedule = new Schedule();
            newSchedule.job = j;
            newSchedule.type = type;
            newSchedule.period = date;
            newSchedule.date = currentTime + date;
            newSchedule.interruptionCode = interruptionCode;
            newSchedule.frame = frame;

            Console.WriteLine("\nCreating new interruption for time: " + newSchedule.date + "\n");

            int index = 0;
            if (queue.Count > 0)
            {
                while (index < queue.Count && newSchedule.date > queue[index].date)
                {
                    index++;
                }
            }

            queue.Insert(index, newSchedule); // insere ordenado
        }

    }

}
