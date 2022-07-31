using System;
using System.Collections.Generic;
using System.Text;

namespace SO_T1
{
    class JobManager // Escalonador
    {
        public static List<Job> jobs = new List<Job>();
        public static List<Job> finishedJobs = new List<Job>();

        private static Job currentJob; // job em execucao

        public static void SetJobList(List<Job> j) // enche o gerenciador com a lista de jobs
        {
            jobs = j;
        }

        public static int JobsCount()
        {
            return jobs.Count;
        }

        public static void InitNextJobOnCPU() // inicilizar CPU com os dados de um job
        {
            if (jobs.Count == 0) // nao ha mais nenhum job na lista
            {
                SO.FinishExecution();
            }

            float maxPriority = 1;
            Job targetJob = null;

            foreach (Job j in jobs) // percorre a lista de processos
            {
                if (j.IsReady()) // caso o processo esteja pronto
                {
                    if(j.priority < maxPriority) // maior prioridade
                    {
                        maxPriority = j.priority; // atualiza a prioridade referencia
                        targetJob = j; // seta o processo
                        //Console.WriteLine("\n" + targetJob.programName + " has the greatest probability: " + targetJob.priority + "\n");
                    }
                }
            }

            if(targetJob != null)
            {
                if (!targetJob.isInitialized) { targetJob.Init(); } // inicializa o job
                targetJob.PutJobOnCPU(); // colocar os dados do job na CPU
                if(currentJob != targetJob) { SO.jobChangeCount++; } // atualiza o contador de troca de processos
                currentJob = targetJob; // atualiza o processo que esta em execucao atualmente
            }
            
        }

        public static Job GetCurrentJob()
        {
            /*if (jobs.Count == 0) // nao ha mais nenhum job na lista
            {
                SO.FinishExecution();
                return null;
            }

            foreach(Job j in jobs)
            {
                if(j.IsReady()) { return j; }
            }

            return null;*/

            return currentJob;

        }

        public static void RemoveJob(Job j)
        {
            jobs.Remove(j); // remove da lista de processos ativos
            finishedJobs.Add(j); // adiciona na lista de processos finalizados
        }

        public static void SetCurrentJobStatus(int state) // atualiza o estado do job atual
        {
            Job j = GetCurrentJob();
            j.UpdateJobStatus(state);
        }

    }
}
