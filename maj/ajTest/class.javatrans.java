package maj.ajTest;


import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class classtest {
	public static void main(String[] myargs)  {
		ASTObject class0 = new ClassDec(null, "foo", null, new TypeDs(new ResolvedTypeD[] {new ResolvedTypeD(new MajUncheckedType(new Identifier("interface1"))), new ResolvedTypeD(new MajUncheckedType(new Identifier("interfac2")))}), new Decs(new Dec[] {new MethodDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))), "foo", new Formals(new FormalDec[] {new FormalDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))), "x"), new FormalDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))), "y")}), new TypeDs(new ResolvedTypeD[] {new ResolvedTypeD(new MajUncheckedType(new Identifier("exceptino1"))), new ResolvedTypeD(new MajUncheckedType(new Identifier("exception2")))}), new CodeBody(new Stmts(new Stmt[] {new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), new Identifier("z")), new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++")), new ReturnStmt(new VarExpr(new Identifier("x")))})))}));
		ASTObject inter1 = new InterfaceDec(null, "foo", null, new Decs(new Dec[] {new VarDec(new Modifiers(1), new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), new Identifier("x")), new MethodDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))), "foo", new Formals(new FormalDec[] {new FormalDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))), "x"), new FormalDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))), "y")}), null, new CodeBody(new Stmts(new Stmt[] {new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), new Identifier("z")), new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++")), new ReturnStmt(new VarExpr(new Identifier("x")))})))}));
		System.out.println(class0.unparse());
		System.out.println(inter1.unparse());
	}
}
