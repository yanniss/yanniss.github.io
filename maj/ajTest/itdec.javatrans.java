package maj.ajTest;


import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class itdectest {
	public static void main(String[] myargs)  {
		ASTObject idec1 = new ParentsDec(new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("Foo")))), new ResolvedTypeD(new MajUncheckedType(new Identifier("Bar"))));
		ASTObject idec2 = new ParentsDec(new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("Foo")))), new TypeDs(new ResolvedTypeD[] {new ResolvedTypeD(new MajUncheckedType(new Identifier("Bar1"))), new ResolvedTypeD(new MajUncheckedType(new Identifier("Bar2")))}));
		ASTObject idec3 = new WarningDec(new AndPcd(new FieldSetPcd(new FieldPattern(null, new AnyTypeName(), null, new SimpleName(new Identifier(new Identifier("Point"), new Identifier("*"))))), new NotPcd(new WithinPcd(new SimpleTypeName(new MajUncheckedType("Point"))))), "\"bad set\"");
		ASTObject idec4 = new ErrorDec(new CallPcd(new ConstructorPattern(null, new SimpleName(new Identifier("Singleton")), new FormalsPattern(new GenTypeNames(new GenTypeName[] {new EllipsesFakeTypeName()})), null)), "\"bad construction\"");
		ASTObject idec5 = new SoftDec(new ResolvedTypeD(new MajUncheckedType(new Identifier("IOException"))), new ExecutionPcd(new ConstructorPattern(null, new SimpleName(new Identifier("Foo")), new FormalsPattern(new GenTypeNames(new GenTypeName[] {new EllipsesFakeTypeName()})), null)));
		ASTObject idec6 = new PrecedenceDec(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("Security")))), new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("Logging"))))}));
		System.out.println(idec1.unparse());
		System.out.println(idec2.unparse());
		System.out.println(idec3.unparse());
		System.out.println(idec4.unparse());
		System.out.println(idec5.unparse());
		System.out.println(idec6.unparse());
	}
}
