import java.io.*;
import java.lang.reflect.*;
//import org.aspectj.compiler.base.ast.*;

public class CreateAspects {
    
    public static void main (String[] myArgs) throws Exception {

	String className = myArgs[0];

	Class thisClass = Class.forName(className);

	Constructor[] cons = thisClass.getDeclaredConstructors();
	Method[] methods = thisClass.getDeclaredMethods();

	for (int i=0; i<methods.length; i++) {
	    String name = methods[i].getName();
	    Class returnType = methods[i].getReturnType();
	    Class[] params = methods[i].getParameterTypes();

	    //	    Identifier nameId = new Identifier(name);
	    

	}

	String aspectName = className + "_Aspect";

	    infer pointcut = `[execution ( )];	
    }

}
