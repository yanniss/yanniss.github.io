package maj.ajTest;


import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class advicetest {
	public static void main(String[] myargs)  {
		BeforeAdviceDec adv1 = new BeforeAdviceDec(null, new Formals(new FormalDec[] {}), null, new FieldGetPcd(new FieldPattern(null, new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int")))), null, new SimpleName(new Identifier(new Identifier("Foo"), new Identifier("y"))))), new CodeBody(new Stmts(new Stmt[] {})));
		System.out.println(adv1.unparse());
	}
}
