using System;
using System.Collections.Generic;
using System.Text;

namespace SO_T2
{
    class Memory
    {
        public static Page[] dataMemory; // memoria fisica
        public static int totalMemorySize = 12; // tamanho total da memoria
        public static int pageSize = 1; // tamanho da pagina
        public static int frameSize = pageSize; // tamanho do quadro

        public static int pageQtd = totalMemorySize / pageSize;

        public static void Init()
        {
            dataMemory = new Page[pageQtd];
            for (int i = 0; i < dataMemory.Length; i++)
            {
                dataMemory[i] = new Page();
            }
        }
    }
}
