using System;
using System.Collections.Generic;
using System.Text;

namespace SO_T2
{
    class Page
    {
        public int[] content; // conteudo dessa pagina

        public bool isAvaliable { get; set; }

        public Page()
        {
            content = new int[Memory.pageSize];
            isAvaliable = true;
        }

        public void Clean()
        {
            Array.Clear(content, 0, content.Length);
            isAvaliable = true;
        }
    }
}
