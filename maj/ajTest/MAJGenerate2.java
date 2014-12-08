package maj.ajTest;

/**
 * @author David Zook
 *
 * Class:
 * Purpose:
 */
import java.io.*;
import java.lang.reflect.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class MAJGenerate2Test {
    
    public static void generateSerializableAspect(Class inClass, PrintStream out) {
	
	// Create new Aspect
	infer serializedAspect = `[aspect SerializableAspect {}];
	
	// Add Serializable to every type used in a method
	for (int meth = 0; meth < inClass.getMethods().length; meth++) {
	    Class[] methSignature = inClass.getMethods()[meth].getParameterTypes();

	    for (int parm = 0; parm < methSignature.length; parm++) {

		Class[] interfaces = methSignature[parm].getInterfaces();
		boolean isSer = false;
		for ( int i=0; i<interfaces.length; i++) {
		    if (interfaces[i].getName().equals("java.io.Serializable")) {
			isSer = true;
			break;
		    }
		}

		if (!methSignature[parm].isPrimitive() && !isSer ) {
		    
		    serializedAspect.addMember(
					       `[	declare parents:
#(Identifier)[new Identifier(methSignature[parm].getName())]
							implements java.io.Serializable;
					       ]
					       );
		}
	    }
	}

	infer compU = `[package gotech.extensions;

			import java.io.*;
			
			#(AspectDec)serializedAspect ];

	out.print(compU.unparse());
    }
    
    
    public static void main(String[] myargs) {
	try {
	    generateSerializableAspect(Class.forName(myargs[0]),
				       System.out);
	} catch (Exception e) { 
	System.out.println("exception : " + e.getMessage());}
    }
}
