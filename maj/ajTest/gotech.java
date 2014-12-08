/**
 * @author David Zook
 *
 * Class:
 * Purpose:
 */
import java.io.*;
import java.lang.reflect.*;
import org.aspectj.compiler.base.ast.*;


public class ExampleMAJ {

    
    public static void main (String[] myArgs) {
	String className = myArgs[0];
	String origName = myArgs[1];
	String jndiName = myArgs[2];

	String aspectName = className + "_Aspect";
	
    }
    /*    public static void main(String[] args) {
	Foo x = new Foo();
	generateSerializableAspect(x.getClass(), System.out);
	generateBean(x.getClass(), System.out);
	}*/
}
