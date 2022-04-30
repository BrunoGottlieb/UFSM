package ibd2.transaction;

import java.util.LinkedList;

public class BrunoConcurrencyManager extends ConcurrencyManager{

	private String method; // Wait and Wound
	private LinkedList<Transaction> confliter = new LinkedList<Transaction>(); // lista que armazena as transacoes conflitantes
	private LinkedList<Transaction> ageList = new LinkedList<Transaction>(); // lista que armazena a ordem de idade
	private boolean implementMultiKill = true; // controla se sera utilizado a implementacao de matar multiplas transacoes no Wound-Wait
	
	public BrunoConcurrencyManager(boolean preemptive) throws Exception {
		super();
		method = preemptive ? "Wound" : "Die"; 
	}

	public Transaction addToQueue(Item item, Instruction instruction) 
	{
		
        Transaction t = instruction.getTransaction();
        Lock l = new Lock(t, instruction.getMode());
        
        if(instruction.getTransaction().getCurrentInstructionIndex() == 0) // caso a transacao seja nova
        {
    		ageList.addFirst(instruction.getTransaction()); // adiciona a nova transacao na lista de idade
    	}
        
        instruction.setItem(item);
        item.locks.add(l); // adiciona lock nessa instrucao
        
        if(method == "Wound") 
        {
        	if(item.locks.size() > 0 && CheckConflict(item, instruction)) // confere se houve conflito
        	{
        		
        		// sou mais velho que confliter e irei mata-lo
        		if(instruction.getTransaction().getCurrentInstructionIndex() >  confliter.getFirst().getCurrentInstructionIndex())
        		{
        			
        			if(implementMultiKill) // caso foi optado por utilizar a implementacao de matar multiplas transacoes
        			{       				
        				for(int i = 0 ; i < confliter.size(); i++) // para cada conflito
        				{
        					try {
								ageList.remove(confliter.get(i)); // remove da lista de idade
								abort(confliter.get(i)); // aborta as n operacoes
							} catch (Exception e) {
								e.printStackTrace();
							}
        				}
						return null; // ja abortou todos os conflitos, executa a instrucao	
        			}  
        			
    				ageList.remove(confliter.getFirst()); // remove da lista de idade
        			
        			return confliter.getFirst(); // retorna transcao a ser abortada
        		}
       		
        		// sou mais velho que confliter e irei mata-lo
        		if(instruction.getTransaction().getCurrentInstructionIndex() ==  confliter.getFirst().getCurrentInstructionIndex() && !AmIYounger(instruction.getTransaction(), confliter.getFirst())) 
        		{
        			
        			if(implementMultiKill) // caso foi optado por utilizar a implementacao de matar multiplas transacoes
        			{       				
        				for(int i = 0 ; i < confliter.size(); i++) // para cada conflito
        				{
        					try {
								ageList.remove(confliter.get(i)); // remove da lista de idade
								abort(confliter.get(i)); // aborta as n operacoes
							} catch (Exception e) {
								e.printStackTrace();
							}
        				}
						return null; // ja abortou todos os conflitos, executa a instrucao	
        			}  
        			
    				ageList.remove(confliter.getFirst()); // remove da lista de idade
        			
        			return confliter.getFirst(); // retorna transcao a ser abortada
        		}
        		
        		// confliter eh mais velho e irei esperar
        		if(instruction.getTransaction().getCurrentInstructionIndex() ==  confliter.getFirst().getCurrentInstructionIndex() && AmIYounger(instruction.getTransaction(), confliter.getFirst()))
        		{
        			return null; // irei esperar, retorna null
        		}

        		// confliter eh mais velho e irei esperar
        		if(instruction.getTransaction().getCurrentInstructionIndex() < confliter.getFirst().getCurrentInstructionIndex())
        		{
        			return null; // irei esperar, retorna null
        		}
        	}
        }
        
        if(method == "Die")
        {

        	if(item.locks.size() > 0 && CheckConflict(item, instruction)) // confere se houve conflito
        	{
        		
        		// sou mais velho que confliter e irei esperar
        		if(instruction.getTransaction().getCurrentInstructionIndex() >  confliter.getFirst().getCurrentInstructionIndex())
        		{
        			return null; // irei esperar, retorna null
        		}
       		
        		// sou mais velho que confliter e irei esperar
        		if(instruction.getTransaction().getCurrentInstructionIndex() ==  confliter.getFirst().getCurrentInstructionIndex() && !AmIYounger(instruction.getTransaction(), confliter.getFirst())) 
        		{
        			return null; // irei esperar, retorna null
        		}
        		
        		// confliter eh mais velho e irei me matar
        		if(instruction.getTransaction().getCurrentInstructionIndex() ==  confliter.getFirst().getCurrentInstructionIndex() && AmIYounger(instruction.getTransaction(), confliter.getFirst()))
        		{
        			ageList.remove(instruction.getTransaction()); // me remove da lista de idade
        			return instruction.getTransaction(); // me mato
        		}

        		// Confliter eh mais velho e irei me matar
        		if(instruction.getTransaction().getCurrentInstructionIndex() < confliter.getFirst().getCurrentInstructionIndex()) 
        		{
        			ageList.remove(instruction.getTransaction()); // me remove da lista de idade
        			return instruction.getTransaction(); // me mato
        		}
        	} 
        } 
        
        return null; // nao houve conflito
	}
	
	private boolean CheckConflict(Item item, Instruction instruction) // confere se houve conflito
	{
        confliter.clear(); // deleta dados anteriores da confliter
        for (int i = 0; i < item.locks.size()-1; i++) // percorre todos os locks
        { 
            Lock lock = item.locks.get(i);
            if(!(instruction.getModeType() == "read" && Instruction.getModeType(lock.mode) == "read")) // se ambos forem read nao eh conflito
            {
            	if(lock.transaction != instruction.getTransaction()) // caso nao seja a propria transacao
            		confliter.add(lock.transaction); // transacao conflitante
            }
            
        }
        
        if(confliter.size() > 0) return true; // houve conflito
        
        return false; // nao houve conflito
        
	}
	
	private boolean AmIYounger(Transaction me, Transaction him) // confere se essa transacao eh mais nova que a conflitante
	{
		for(int i = 0; i < ageList.size(); i++) 
		{
			if(ageList.get(i) == me) return true; // sou mais jovem, sou o mais recente da lista
			if(ageList.get(i) == him) return false; // ele eh mais jovem, eh o mais recente da lista
		}
    	
    	System.out.println("Not supposed to be here");
    	
    	return false; // nao eh suposto chegar aqui
	}
	
}
