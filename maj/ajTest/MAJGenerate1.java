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

public class MAJGenerate1 {
	
	
	// Create a serializable declaration for each type in each method in the original class.

	public static void generateSerializableAspect(Class inClass, PrintStream out) {
		
	infer serializedAspect =
		`[	package gotech.extensions;

			import java.io.*;

//			aspect #(Identifier)[new Identifier(inClass.getName()+"SerializableAspect")] {
			aspect SerializableAspect {

				#(Member[])[(new Object() { Members[] eval() { Members[] result;
					int i = 0;
					for (int meth = 0; meth < inClass.getMethods().length; meth++) {
						Class[] methSignature = inClass.getMethods()[meth].getParameterTypes();
						for (int parm = 0; parameter < methSignature.length; parm++) {
							if (!parmType.primitive() &&
								!methSignature[parm].toSource().equals("java.io.Serializable")) {

								result[i++] =
								`[	declare parents:
										#(Identifier)[new Identifier(methSignature[parm].getName())]
											implements java.io.Serializable;
								];
							}
						}
					}
				return result;} }).eval()] // End #Members[]

			}
		];
		out.print(newAspect.toSource());
	}
}
