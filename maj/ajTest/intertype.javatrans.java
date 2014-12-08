package maj.ajTest;


import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class intertypetest {
	public static void main(String[] myargs)  {
		ASTObject id1 = new Identifier("foo");
		ASTObject var1A = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), new Identifier("x"));
		ASTObject var1B = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), (Identifier)id1);
		ASTObject var1C = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), (Identifier)id1);
		ASTObject var2A = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new Identifier("int")))))), new Identifier("x"));
		ASTObject var2B = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new Identifier("int")))))), (Identifier)id1);
		ASTObject var2C = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new Identifier("int")))))), (Identifier)id1);
		ASTObject var3B = new VarDec(null, new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int")))))), (Identifier)id1);
		ASTObject var3C = new VarDec(null, new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int")))))), (Identifier)id1);
		ASTObject var4A = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("Foo"))))), new Identifier("x"));
		ASTObject var4B = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("Foo"))))), (Identifier)id1);
		ASTObject var4C = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("Foo"))))), (Identifier)id1);
		ASTObject var5A = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new Identifier("Foo")))))), new Identifier("x"));
		ASTObject var5B = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new Identifier("Foo")))))), (Identifier)id1);
		ASTObject var5C = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new Identifier("Foo")))))), (Identifier)id1);
		ASTObject var6A = new VarDec(null, new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("Foo")))))), new Identifier("x"));
		ASTObject var6B = new VarDec(null, new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("Foo")))))), (Identifier)id1);
		ASTObject var6C = new VarDec(null, new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("Foo")))))), (Identifier)id1);
		ASTObject var7A = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier(new Identifier("Foo"), new Identifier("Bar")))))), new Identifier("x"));
		ASTObject var7B = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier(new Identifier("Foo"), new Identifier("Bar")))))), (Identifier)id1);
		ASTObject var7C = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier(new Identifier("Foo"), new Identifier("Bar")))))), (Identifier)id1);
		ASTObject var8A = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new Identifier(new Identifier("Foo"), new Identifier("Bar"))))))), new Identifier("x"));
		ASTObject var8B = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new Identifier(new Identifier("Foo"), new Identifier("Bar"))))))), (Identifier)id1);
		ASTObject var8C = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new Identifier(new Identifier("Foo"), new Identifier("Bar"))))))), (Identifier)id1);
		ASTObject var9A = new VarDec(null, new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier(new Identifier("Foo"), new Identifier("Bar"))))))), new Identifier("x"));
		ASTObject var9B = new VarDec(null, new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier(new Identifier("Foo"), new Identifier("Bar"))))))), (Identifier)id1);
		ASTObject var9C = new VarDec(null, new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier(new Identifier("Foo"), new Identifier("Bar"))))))), (Identifier)id1);
		System.out.println(id1.unparse());
		System.out.println(var1A.unparse());
		System.out.println(var1B.unparse());
		System.out.println(var1C.unparse());
		System.out.println(var2A.unparse());
		System.out.println(var2B.unparse());
		System.out.println(var2C.unparse());
		System.out.println(var3B.unparse());
		System.out.println(var3C.unparse());
		System.out.println(var4A.unparse());
		System.out.println(var4B.unparse());
		System.out.println(var4C.unparse());
		System.out.println(var5A.unparse());
		System.out.println(var5B.unparse());
		System.out.println(var5C.unparse());
		System.out.println(var6A.unparse());
		System.out.println(var6B.unparse());
		System.out.println(var6C.unparse());
		System.out.println(var7A.unparse());
		System.out.println(var7B.unparse());
		System.out.println(var7C.unparse());
		System.out.println(var8A.unparse());
		System.out.println(var8B.unparse());
		System.out.println(var8C.unparse());
		System.out.println(var9A.unparse());
		System.out.println(var9B.unparse());
		System.out.println(var9C.unparse());
	}
}
