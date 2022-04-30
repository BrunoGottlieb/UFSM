package ibd2.query;

import ibd2.Block;

public class BrunoGottliebBlockNestedLoopJoin implements Operation {

	Operation op1;
    Operation op2;

    Tuple curTuple1;
    Tuple curTuple2;
    Tuple nextTuple;
    
    Tuple[] joinBuffer = new Tuple[Block.RECORDS_AMOUNT.intValue()]; // declara o buffer do tamanho do bloco
	
    int currentIndex = 0; // posicao atual do buffer
    int bufferSize = 0; // tamanho do buffer sendo utilizado
    
	public BrunoGottliebBlockNestedLoopJoin(Operation op1, Operation op2) throws Exception {
        super();
        this.op1 = op1;
        this.op2 = op2;
    }
	
	@Override
	public void open() throws Exception 
	{
        op1.open();
        op2.open();
        curTuple1 = null;
        curTuple2 = null;
        nextTuple = null;
	}

	@Override
	public Tuple next() throws Exception 
	{
		if (nextTuple != null) // retorna o next value, caso ja tenha sido pego
        {
            Tuple next_ = nextTuple;
            nextTuple = null;
            return next_;
        }
		
		if(bufferSize == 0) 
		{
			FillBuffer(); // preenche o buffer  
			//PrintBuffer(); // exibe os valores do buffer apenas por motivos de debug
			op2.open();
		}

		while(op2.hasNext() || curTuple2 != null) // confere se ainda ha valores na tabela 2
		{
        	if(curTuple2 == null) 
        	{
        		curTuple2 = (Tuple)op2.next(); // chama o proximo valor da tabela 2
        		op1.open();
        	}
        			
			for(int i = currentIndex; i < joinBuffer.length; i++) // percorre todo o buffer
            {
				
				System.out.println("Comparando: " + joinBuffer[i].primaryKey + " e " + curTuple2.primaryKey);
            
				if (joinBuffer[i].primaryKey == curTuple2.primaryKey) // caso seja um match
                {
					Tuple rec = new Tuple();
                    rec.primaryKey = joinBuffer[i].primaryKey;
                    rec.content = joinBuffer[i].content + " " + curTuple2.content;
                    return rec;
                }
				
				if(i == bufferSize-1) // chegou ao final do buffer
				{
					currentIndex = 0; // reinicia o index caso ele chegou ao final	
					curTuple2 = null;
					break; // sai do laco for
				}
				
            }

		}

        throw new Exception("No record after this point");

	}

	@Override
	public boolean hasNext() throws Exception 
	{

		do {
		
			if (nextTuple != null)
	            return true;
			
			if(bufferSize == 0) 
			{
				FillBuffer(); // preenche o buffer  
				//PrintBuffer(); // exibe os valores do buffer apenas por motivos de debug
				op2.open();
			}
	
	        while(curTuple2 != null || op2.hasNext()) // confere se ainda ha valores na tabela 2
			{
	        	if(curTuple2 == null) 
	        	{
	        		curTuple2 = (Tuple)op2.next(); // chama o proximo valor da tabela 2
	        	}
	        			
				for(int i = currentIndex; i < joinBuffer.length; i++) // percorre todo o buffer
	            {
					
					//System.out.println("Comparando: " + joinBuffer[i].primaryKey + " e " + curTuple2.primaryKey);
	            
					if (joinBuffer[i].primaryKey == curTuple2.primaryKey) // caso seja um match
	                {
	                    nextTuple = new Tuple();
	                    nextTuple.primaryKey = joinBuffer[i].primaryKey;
	                    nextTuple.content = joinBuffer[i].content + " " + curTuple2.content;
	                    
	                    if(i == bufferSize-1) // chegou ao final do buffer
	    				{
	    					currentIndex = 0; // reinicia o index caso ele chegou ao final
	    					curTuple2 = null;
	    				}
	                    else currentIndex = i+1;
	                    return true;
	                }
					
					if(i == bufferSize-1) // chegou ao final do buffer
					{
						currentIndex = 0; // reinicia o index caso ele chegou ao final	
						curTuple2 = null;
						break; // sai do laco for
					}
					
	            }
	
			}
	        
	        ResetBuffer();
	        
	        FillBuffer();
	
	        //PrintBuffer();
	        
	        op2.open();
        
		} while(bufferSize > 0);
        
        return false; // acabou de varrer as duas tabelas
	}

	private void ResetBuffer() 
	{
		for(int i = 0; i < joinBuffer.length; i++)
			joinBuffer[i] = null;
		bufferSize = 0;
	}
	
	private void PrintBuffer()
	{
		System.out.println("BufferSize: " + bufferSize);
		for(int i = 0; i < joinBuffer.length; i++) 
		{
			if(joinBuffer[i] == null) return;
			System.out.println("i: " + joinBuffer[i].primaryKey);
		}
	}
	
	private void FillBuffer() throws Exception 
	{
		for(int i = 0; i < joinBuffer.length; i++) 
		{
			if(op1.hasNext()) 
			{
				joinBuffer[i] = op1.next();
				curTuple1 = joinBuffer[i];
				bufferSize = i+1;
			} else return;
		}
	}

	@Override
	public void close() throws Exception {
		// TODO Auto-generated method stub

	}
	
    public Operation getLeftOperation() {
        return op1;
    }

    public Operation getRigthOperation() {
        return op2;
    }

}
