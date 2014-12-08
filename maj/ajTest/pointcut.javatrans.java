package maj.ajTest;


import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class pointcuttest {
	public static void main(String[] myargs)  {
		ASTObject pct1 = new CallPcd(new MethodPattern(new ModifiersPattern(9, 0), null, new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("void")))), new SimpleName(new Identifier(new Identifier("Foo"), new Identifier("m"))), new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int")))), new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int")))), new EllipsesFakeTypeName()})), null));
		ASTObject pct2 = new CallPcd(new ConstructorPattern(null, new SimpleName(new Identifier("Foo")), new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))})), null));
		ASTObject pct3 = new ExecutionPcd(new MethodPattern(null, null, new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("void")))), new SimpleName(new Identifier(new Identifier("Foo"), new Identifier("m"))), new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))})), null));
		ASTObject pct4 = new ExecutionPcd(new ConstructorPattern(null, new SimpleName(new Identifier("Foo")), new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))})), null));
		ASTObject pct5 = new InitializerPcd(new ConstructorPattern(null, new SimpleName(new Identifier("Foo")), new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))})), null));
		ASTObject pct6 = new PreInitializerPcd(new ConstructorPattern(null, new SimpleName(new Identifier("Foo")), new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))})), null));
		ASTObject pct7 = new StaticInitializerPcd(new ConstructorPattern(null, new SimpleName(new Identifier("Foo")), new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))})), null));
		ASTObject pct8 = new FieldGetPcd(new FieldPattern(null, new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("Foo")))), null, new SimpleName(new Identifier("x"))));
		ASTObject pct9 = new FieldSetPcd(new FieldPattern(null, new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("Foo")))), null, new SimpleName(new Identifier("x"))));
		ASTObject pct10 = new HandlerPcd(new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("IOException")))));
		ASTObject pct11 = new AdviceExecutionPcd();
		ASTObject pct12 = new WithinPcd(new SimpleTypeName(new MajUncheckedType("Foo.Bar.*")));
		ASTObject pct13 = new WithinCodePcd(new MethodPattern(null, null, new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("void")))), new SimpleName(new Identifier(new Identifier("Foo"), new Identifier("m"))), new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))})), null));
		ASTObject pct14 = new WithinCodePcd(new ConstructorPattern(null, new SimpleName(new Identifier("Foo")), new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))})), null));
		ASTObject pct15 = new WithinCodePcd(new MethodPattern(new ModifiersPattern(0, 2), null, new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("void")))), new SimpleName(new Identifier(new Identifier("Foo"), new Identifier("*"))), new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))})), null));
		ASTObject pct20 = new CFlowPcd(new CallPcd(new MethodPattern(null, null, new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("void")))), new SimpleName(new Identifier(new Identifier("Foo"), new Identifier("m"))), new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))})), null)));
		ASTObject pct21 = new CFlowBelowPcd(new CallPcd(new MethodPattern(null, null, new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("void")))), new SimpleName(new Identifier(new Identifier("Foo"), new Identifier("m"))), new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))})), null)));
		ASTObject pct24 = new TargetPcd(new Identifier(new Identifier(new Identifier("java"), new Identifier("io")), new Identifier("InputPort")));
		ASTObject pct25 = new ArgsPcd(new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier(new Identifier(new Identifier("java"), new Identifier("io")), new Identifier("InputPort"))))), new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))})));
		ASTObject pct26 = new ArgsPcd(new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int")))), new AnyTypeName()})));
		ASTObject pct27 = new ArgsPcd(new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int")))), new EllipsesFakeTypeName(), new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))})));
		ASTObject pct28 = new ThisPcd(new Identifier("Point"));
		ASTObject pct29 = new IfPcd(new NumericTestOpExpr(new VarExpr(new Identifier("x")), "==", new VarExpr(new Identifier("y"))));
		ASTObject pct30 = new NotPcd(new ArgsPcd(new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int")))), new EllipsesFakeTypeName(), new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))}))));
		ASTObject pct31 = new AndPcd(new ArgsPcd(new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int")))), new EllipsesFakeTypeName(), new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))}))), new WithinCodePcd(new ConstructorPattern(null, new SimpleName(new Identifier("Foo")), new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))})), null)));
		ASTObject pct32 = new OrPcd(new ArgsPcd(new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int")))), new EllipsesFakeTypeName(), new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))}))), new WithinCodePcd(new ConstructorPattern(null, new SimpleName(new Identifier("Foo")), new FormalsPattern(new GenTypeNames(new GenTypeName[] {new SimpleTypeName(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))})), null)));
		ASTObject pct35 = new IfPcd(new NumericTestOpExpr(new VarExpr(new Identifier("x")), "==", new VarExpr(new Identifier("y"))));
		System.out.println(pct1.unparse());
		System.out.println(pct2.unparse());
		System.out.println(pct3.unparse());
		System.out.println(pct4.unparse());
		System.out.println(pct5.unparse());
		System.out.println(pct6.unparse());
		System.out.println(pct7.unparse());
		System.out.println(pct8.unparse());
		System.out.println(pct9.unparse());
		System.out.println(pct10.unparse());
		System.out.println(pct11.unparse());
		System.out.println(pct12.unparse());
		System.out.println(pct13.unparse());
		System.out.println(pct14.unparse());
		System.out.println(pct15.unparse());
		System.out.println(pct20.unparse());
		System.out.println(pct21.unparse());
		System.out.println(pct24.unparse());
		System.out.println(pct25.unparse());
		System.out.println(pct26.unparse());
		System.out.println(pct27.unparse());
		System.out.println(pct28.unparse());
		System.out.println(pct29.unparse());
		System.out.println(pct30.unparse());
		System.out.println(pct31.unparse());
		System.out.println(pct32.unparse());
		System.out.println(pct35.unparse());
	}
}
