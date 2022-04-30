/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ibd2.query;

/**
 *
 * @author SergioI
 */
public class BrunoUnion  implements BinaryOperation{

    Operation op1;
    Operation op2;

    Tuple curTuple1;
    Tuple curTuple2;
    Tuple nextTuple;

    public BrunoUnion(Operation op1, Operation op2) throws Exception {
        super();
        this.op1 = op1;
        this.op2 = op2;
    }
    
    @Override
    public void open() throws Exception{

        op1.open();
        op2.open();
        curTuple1=null;
        curTuple2=null;
        nextTuple = null;

    }

    @Override
    public Tuple next() throws Exception
    {
    	if (nextTuple!=null) // caso ja tenha a proxima tupla calculada pelo hasNext
    	{
            Tuple next_ = nextTuple;
            nextTuple = null;
            return next_; // ja retorna com o resultado
        }

    	// caso ainda nao tenha sido calculado pelo hasNext
    	if (curTuple1==null)
    	{
            if (op1.hasNext())
            	curTuple1 = op1.next();
        }

        if (curTuple2==null)
        {
        	if (op2.hasNext())
            	curTuple2 = op2.next();
        }

        while (true)
        {
            if (curTuple1 == null && curTuple2 != null) 
            {
            	Tuple rec = new Tuple();
            	rec.primaryKey = curTuple2.primaryKey;
                rec.content = curTuple2.content;
                curTuple2 = null;
                return rec;
            }
            else if (curTuple2 == null && curTuple1 != null) 
            {
            	Tuple rec = new Tuple();
            	rec.primaryKey = curTuple1.primaryKey;
                rec.content = curTuple1.content;
                curTuple1 = null;
                return rec;
            } 
            else if (curTuple1.primaryKey==curTuple2.primaryKey)
            {
                Tuple rec = new Tuple();
                rec.primaryKey = curTuple1.primaryKey;
                rec.content = curTuple1.content+" "+curTuple2.content;
                curTuple1 = null;
                curTuple2 = null;
                return rec;
            }
            else // ambos sao nulos, nao ha proximo
            {
            	break;
            }

        }
        throw new Exception("No more data");

    }


    @Override
    public boolean hasNext() throws Exception {
    	
    	if (curTuple1==null)
    	{
            if (op1.hasNext()) // confere se ha um proximo registro na operacao 1
            	return true;
            	//curTuple1 = op1.next(); // deixa engatilhado
        }

        if (curTuple2==null)
        {
        	if (op2.hasNext())
        		return true;
            	//curTuple2 = op2.next();
        }
        
        // ambos sao nulos, nao ha proximo
        if(curTuple1 == null && curTuple2 == null) return false;

        // Nao vai chegar aqui, retirei essa parte debaixo para que o iterador nao avance
        // ao ser chamado hasNext consecutivas vezes antes de chamar um Next
        // Porem pode ser utilizado trocando os comentarios ali em cima
        
        while (true)
        {
            if (curTuple1 == null && curTuple2 != null) 
            {
            	nextTuple = new Tuple();
                nextTuple.primaryKey = curTuple2.primaryKey;
                nextTuple.content = curTuple2.content;
                curTuple2 = null;
                return true;
            }
            else if (curTuple2 == null && curTuple1 != null) 
            {
            	nextTuple = new Tuple();
                nextTuple.primaryKey = curTuple1.primaryKey;
                nextTuple.content = curTuple1.content;
                curTuple1 = null;
                return true;
            } 
            else if (curTuple1.primaryKey == curTuple2.primaryKey)
            {
                nextTuple = new Tuple();
                nextTuple.primaryKey = curTuple1.primaryKey;
                nextTuple.content = curTuple1.content+" "+curTuple2.content;
                curTuple1 = null;
                curTuple2 = null;
                return true;
            }
            else 
            {
            	return false; // nao ha um proximo pois ambos sao nulos
            }
        }
    }
        
    @Override
    public void close() {

    }

    @Override
    public Operation getLeftOperation() {
        return op1;
    }

    @Override
    public Operation getRigthOperation() {
        return op2;
    }

}
