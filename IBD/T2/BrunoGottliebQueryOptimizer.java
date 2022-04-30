package ibd2.query;

import java.util.ArrayList;

public class BrunoGottliebQueryOptimizer 
{
	
	ArrayList<Operation> operations = new ArrayList<Operation>(); // lista que guardara as operacoes
	boolean allMerges = true; // confere se todas as juncoes sao de MergeJoin
	
	public Operation optimizeQuery(Operation op) throws Exception {
		
		System.out.println("The process has began. Please wait...\n");
		
		operations.clear(); allMerges = true;
		
		recursiveMethod(op); // metodo recursivo que ira preencher a arrayList de operacoes
		
		if (allMerges) // caso eram todos MergeJoins, apenas retorna
		{ 
			return op;	
		}
		
		while (operations.size() > 1) // laco que ira juntando as tabelas ate ficar apenas o nivel mais alto
		{
			Operation smaller = getSmallerOperation(); // encontra o menor
			
			Operation smaller2 = getSmallerOperation(); // encontra o segundo menor
			
			Operation join = new NestedLoopJoin(smaller, smaller2); // une ambos utilizando NestedLoopJoin
			
			operations.add(join); // adiciona este nivel a lista de operacoes
			
		}

		return operations.get(0); // retorna a operacao de nivel mais alto
		
	}
	
	private Operation getSmallerOperation() throws Exception // retorna a menor operacao da lista
	{
		Operation target = null; // armazena temporariamente a menor operacao
		int tamMenor = -1;
		int pos = -1; // posicao da operacao a ser excluida
		
		for(int i = 0; i < operations.size(); i++) // percorre a lista de operacoes
		{
			int operationSize = getSize(operations.get(i));
			if(tamMenor == -1 || operationSize < tamMenor) // encontra a menor operacao
			{
				tamMenor = operationSize;
				target = operations.get(i);
				pos = i;
			}
		}
		operations.remove(pos);
		return target;
	}
	
	private void recursiveMethod(Operation op) throws Exception // distribui as operacoes para a lista
	{
		op.open();
		
		if (op instanceof NestedLoopJoin) 
		{
			NestedLoopJoin nested = (NestedLoopJoin)op;
			Operation left = nested.getLeftOperation();
			Operation right = nested.getRigthOperation();
			recursiveMethod(left);
			recursiveMethod(right);
			allMerges = false;
		} 
		else if (op instanceof MergeJoin) 
		{
			MergeJoin merge = (MergeJoin)op;
			Operation left = merge.getLeftOperation();
			Operation right = merge.getRigthOperation();
			recursiveMethod(left);
			recursiveMethod(right);
		}
		else 
		{	
			operations.add(op); // adiciona esta operacao a lista
			op.close();
			return;
		}
		return;
	}
	
	private int getSize(Operation op) throws Exception // conta quantos registros tem dentro dessa operacao
	{
		op.open();
		int index = 0;
		while (op.hasNext()){
			Tuple r = op.next();
	        index++;
        }
		op.close();
		return index;
	}
	
}
