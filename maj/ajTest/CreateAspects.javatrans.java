import java.io.*;
import java.lang.reflect.*;

public class CreateAspects {
	public static void main(String[] myArgs) throws Exception {
		 className = myArgs[0];
		 thisClass = Class.forName(className);
		[] cons = thisClass.getDeclaredConstructors();
		[] methods = thisClass.getDeclaredMethods();
		for (int i = 0; i < methods.length; i++) {
			 name = methods[i].getName();
			 returnType = methods[i].getReturnType();
			[] params = methods[i].getParameterTypes();
			System.out.println("name: " + name);
			System.out.println("return: " + returnType.getName());
			for (int j = 0; j < params.length; j++) {
				System.out.println("param: " + params[j].getName());
			}
		}
		 aspectName = className + "_Aspect";
	}
}
