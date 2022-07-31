using System;
using System.Collections.Generic;
using System.Text;

namespace SO_T2
{
    class MMU
    {
        /// Para todo acesso válido a uma página, a MMU deve alterar o indicador de acesso correspondente;
        /// se o acesso for de alteração, deve também alterar o indicador de alteração.Essas são as únicas
        /// alterações que a MMU pode fazer na tabela de páginas.

        ///  O número de entradas nesse vetor deve corresponder ao tamanho máximo de um espaço de endereçamento virtual
        public static PageInfo[] pagesTable = new PageInfo[100]; // tabela de páginas

        // obter o tamanho total da memória 
        public static int GetTotalMemorySize()
        {
            return Memory.totalMemorySize;
        }

        // obter o tamanho de um quadro
        public static int GetFrameSize()
        {
            return Memory.frameSize;
        }

        public static int GetNextFreeFrame() // retorna o proximo quadro livre da memoria fisica
        {
            for(int i = 0; i < Memory.dataMemory.Length; i++)
            {
                if(Memory.dataMemory[i].isAvaliable == true)
                {
                    return i; // retorna o quadro livre
                }
            }

            return -1; // Nao ha quadros livres
        }

        public static bool CheckPageFault(int index)
        {
            int frame = index / Memory.pageSize; // numero do quadro para onde vai o dado

            Console.WriteLine("Index: " + index + "| Frame: " + frame);

            if (pagesTable[frame].isValid == false) // confere se a pagina esta mapeada
            {
                //Console.WriteLine("\n\nPAGINA NAO MAPEADA: " + frame + "\n\n");
                return true; // retorna erro de falta de pagina
            }

            return false;
        }

        public static bool CheckViolation(int index)
        {
            /*if (index >= Memory.totalMemorySize || index < 0) // endereco de memoria eh invalido
            {
                Console.WriteLine("\n\nVIOLACAO: " + index + "\n\n");
                return true; // retorna violacao
            }*/

            return false;
        }

        // alterar um inteiro em uma posição de memória
        public static void SetDataMemoryAtIndex(int newData, int index)
        {
            //Console.WriteLine("\n\nSetDataMemoryAtIndex: " + index + "\n\n");

            int frame = index / Memory.pageSize; // numero do quadro para onde vai o dado
            int offset = index - (frame * Memory.pageSize); // deslocamento dentro do quadro

            PageInfo paginaDescritor = pagesTable[frame]; // pagina do processo para onde vai o dado
            paginaDescritor.wasAccessed = true;
            paginaDescritor.wasChanged = true;
            paginaDescritor.isValid = true;
            Page page = Memory.dataMemory[paginaDescritor.frameNum]; // frame na memoria fisica, o descritor contem essa info ja do SO
            page.content[offset] = newData; // deslocamento dentro da pagina | recebe novo dado

            /*Console.WriteLine("\n\n\n--------MEMORY---------\n\n\n");

            foreach(Page p in Memory.dataMemory)
            {
                Console.WriteLine(p);
                foreach (int i in p.content)
                {
                    Console.WriteLine(i);
                }
            }

            Console.WriteLine("\n\n\n");*/

        }

        // ler um inteiro de uma posição de memória
        public static int GetDataMemoryByIndex(int index)
        {
            //Console.WriteLine("\n\nGetDataMemoryByIndex: " + index + "\n\n");

            int frame = index / Memory.pageSize; // numero do quadro para onde vai o dado
            int offset = index - (frame * Memory.pageSize); // deslocamento dentro do quadro

            PageInfo paginaDescritor = pagesTable[frame]; // pagina do processo para onde vai o dado na tabela de paginas
            Page pagina = Memory.dataMemory[paginaDescritor.frameNum]; // pagina do processo para onde vai o dado na memoria principal
            paginaDescritor.wasAccessed = true;
            return pagina.content[offset]; // retorna o dado da memoria na posicao correspondente
        }

        public static void CleanJobFromMemory(Job job) // retira os dados do processo finalizado da memoria
        {
            //Console.WriteLine("Cleaning job: " + job.programName);
            for(int i = 0; i < job.pagesTable.Length; i++)
            {
                if(job.pagesTable[i].isValid)
                {
                    if(job.pagesTable[i].isAtSecondary) // retira os dados da memoria secundaria
                    {
                        SO.secondaryMemory[job.pagesTable[i].posAtSecondary] = new Page();
                    }
                    int frame = job.pagesTable[i].frameNum;
                    if (frame != null && frame >= 0) // retira os dados da memoria primaria
                        Memory.dataMemory[frame].Clean();
                }
            }
        }

        public static void SetMemoryFrameValidity(int index, bool state) // altera a validade de um quadro da memoria fisica
        {
            Memory.dataMemory[index].isAvaliable = state;
        }

    }
}
