package ibd2;

import java.util.Iterator;

public class SlackedOrderedTable extends Table{

    public SlackedOrderedTable() {
        super();
        }
        
        
        public SlackedOrderedTable(String folder, String name) throws Exception {
            super(folder, name);
        }
    
    
        @Override
        protected Long selectBlock(long primaryKey) throws Exception {
    
            IndexRecord ir = getLargestSmallerKey(primaryKey); // pega o maior, que seja menor que o valor a ser inserido
            Block b = null;
            if (ir != null) { // caso o valor exista
                b = getBlock(ir.getBlockId()); // armazena o bloco de destino
            } else {
                b = getBlock(0L); //
            }

            if (b.isFull()) { // caso o bloco esteja cheio, iniciar acao recursiva

                //System.out.println("Bloco que iria inserir esta cheio: " + b.block_id + "\n");
    
                Record[] maioresRecords = getLargestValues(b); // encontra os maiores, que serao chutados para o bloco seguinte

                //System.out.println("Qtd para remover: " + maioresRecords.length);

                for (Record r : maioresRecords) { // remove esses valores
                    if(r != null){
                        //System.out.println("Ira remover: " + r.getPrimaryKey());
                        removeRecord(r); 
                    }
                }

                for (Record r : maioresRecords) { // chuta os maiores valores ate um bloco disponivel
                    if(r != null){
                        //System.out.println("Ira adicionar: " + r.getPrimaryKey());
                        recursiveSlide(b, r); 
                    }
                }

                return b.block_id; // retorna o bloco onde o dado sera inserido
            }
            return b.block_id; // retorna o bloco onde o dado sera inserido
        }

        private void recursiveSlide(Block b, Record recRecebido) throws Exception {

            Long next = b.block_id + 1;
            Block b2 = getBlock(next);
    
            if (b2.isFull()) {
                Record[] maioresRecords2 = getLargestValues(b2); // encontra os maiores deste bloco cheio

                for (Record r : maioresRecords2) { // remove esses valores
                    if(r != null){
                        //System.out.println("Ira remover: " + r.getPrimaryKey());
                        removeRecord(r); 
                    }
                }
    
                addRecord(b2, recRecebido); // agora adiciona o valor recebido no bloco que era pra ser
    
                for (Record r : maioresRecords2) { // para os maiores valores que foram retirados do bloco
                    if(r != null){
                        //System.out.println("Ira adicionar: " + r.getPrimaryKey());
                        recursiveSlide(b2, r); // chuta os maiores valores ate um bloco disponivel
                    }
                }

            } else { // tem espaco, adiciona o dado ao bloco

                addRecord(b2, recRecebido);
    
            }
        }

        private Record[] getLargestValues(Block b) throws Exception { // retorna os maiores valores do bloco
    
            Long max = -1L;
            Record maxR = null;
            Iterator<Record> it = b.iterator();

            Record[] vet = new Record[b.getRecordsCount()]; // cria um vetor de tamanho do bloco

            int index = 0;
            while (it.hasNext()){ // preenche o vetor com todos os valores deste bloco
                Record rec = it.next();
                vet[index] = rec;
                index++;
            }

            vet = orderArray(vet); // ordena o vetor em ordem crescente

            //System.out.println("Tam do vet original: " + vet.length);

            Record[] finalVet = new Record[(vet.length/2)+1]; // vetor que sera retornado apenas com a metade maior

            index = 0;
            for(int i = (vet.length/2); i < vet.length; i++){ // preenche o final vet apenas com a metade maior
                finalVet[index] = vet[i];
                index++;
            }

            return finalVet;
        }

        private Record[] orderArray(Record[] vet){ // ordena o vetor
            Record temp;
            for (int i = 1; i < vet.length; i++) 
            {
                for (int j = i; j > 0; j--) 
                {
                    if (vet[j].getPrimaryKey() < vet [j - 1].getPrimaryKey()) 
                    {
                        temp = vet[j];
                        vet[j] = vet[j - 1];
                        vet[j - 1] = temp;
                    }
                }
            }
            return vet;
        }

        private IndexRecord getLargestSmallerKey(long primaryKey) {
            IndexRecord ir = null;
            for (long i = primaryKey; i >= 0; i--) {
                ir = index.getEntry(i);
                if (ir != null) {
                    break;
                }
            }
            return ir;
        }
    
}