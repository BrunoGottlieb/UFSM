using System;
using System.Collections.Generic;
using System.Text;

namespace SO_T2
{
    class PageInfo
    {
        public Job ownJob { get; set; } // processo a qual essa pagina pertence
        public int ownFrame { get; set; } // posicao da pagina no vetor do processo
        public int frameNum { get; set; } // o número do quadro correspondente a pagina
        public bool isValid { get; set; } // uma indicação de validade (o descritor eh valido ou nao)
        public bool isChangeable { get; set; } // indicador se a pagina eh alteravel ou nao
        public bool wasAccessed { get; set; } // indicador se a pagina foi acessada ou nao
        public bool wasChanged { get; set; } // indicador se a pagina foi alterada ou nao
        public bool isAtSecondary { get; set; } // indica se essa pagina esta na memoria secundaria
        public int posAtSecondary { get; set; } // posicao da pagina na memoria secundaria

        public PageInfo()
        {
            frameNum = -1;
            isValid = false;
            isChangeable = true;
            wasAccessed = false;
            wasChanged = false;
            isAtSecondary = false;
        }
    }
}
