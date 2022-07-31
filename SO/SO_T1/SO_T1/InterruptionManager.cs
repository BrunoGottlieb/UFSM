using System;
using System.Collections.Generic;
using System.Text;

namespace SO_T1
{
    class InterruptionManager // Controlador
    {
        private const int normal = 0;
        private const int ilegal = 1;
        private const int violacao = 2;
        private const int sleeping = 3;

        public void Execute()
        {
            Timer.Initialize();

            while (true) // laco que mantem o programa em execucao
            {
                int interruptionCode = CPU.GetCPUInterruptionCode();

                if (interruptionCode == normal)
                {
                    CPU.ExecuteCPU();
                }
                else if (interruptionCode == ilegal)
                {
                    SO.IlegalHandler();
                }
                else if (interruptionCode == violacao)
                {
                    SO.ViolationHandler();
                }
                else if (interruptionCode == sleeping)
                {
                    Console.WriteLine("CPU is sleeping"); // nothing
                }

                // ficar chamando esse metodo enquanto houver interrupcao
                do
                {
                    interruptionCode = Timer.UpdateTime();
                }
                while (interruptionCode != normal);

            }
        }
    }
}
