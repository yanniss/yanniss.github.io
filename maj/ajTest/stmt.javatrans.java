package maj.ajTest;


import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class stmttest {
	public static void main(String[] myargs)  {
		ASTObject stat1 = new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++"));
		ASTObject expr1 = new CallExpr("Bar", new Exprs(new Expr[] {new VarExpr(new Identifier("x"))}));
		ASTObject expr2 = new ClassExpr(new ResolvedTypeD(new MajUncheckedType(new VarExpr(new Identifier("bar")))));
		ASTObject stat3 = new ForStmt(new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), new Identifier("x"), new IntLiteralExpr(0)), new NumericTestOpExpr(new VarExpr(new Identifier("x")), "!=", new IntLiteralExpr(5)), new Exprs(new Expr[] {new PostfixExpr(new VarExpr(new Identifier("x")), "++")}), new CodeBody(new Stmts(new Stmt[] {new ExprStmt(new CallExpr("Bar", new Exprs(new Expr[] {new VarExpr(new Identifier("x"))}))), new ExprStmt(new CallExpr("foo", new Exprs(new Expr[] {new VarExpr(new Identifier("x"))})))})));
		ASTObject var1 = new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))))), new Identifier("x"), new IntLiteralExpr(0));
		ASTObject var2 = new ExprStmt(new NumericTestOpExpr(new VarExpr(new Identifier("x")), "!=", new IntLiteralExpr(5)));
		ASTObject expr3 = new PostfixExpr(new VarExpr(new Identifier("x")), "++");
		ASTObject stat3A = new ForStmt(var1, new NumericTestOpExpr(new VarExpr(new Identifier("x")), "!=", new IntLiteralExpr(5)), new Exprs(new Expr[] {expr3}), new CodeBody(new Stmts(new Stmt[] {new ExprStmt(new CallExpr("Bar", new Exprs(new Expr[] {new VarExpr(new Identifier("x"))})))})));
		ASTObject stat4 = new IfStmt(new NumericTestOpExpr(new VarExpr(new Identifier("x")), "==", new VarExpr(new Identifier("y"))), new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++")), new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("y")), "++")));
		ASTObject exp1 = new NumericTestOpExpr(new VarExpr(new Identifier("x")), "==", new VarExpr(new Identifier("y")));
		ASTObject stat4A = new IfStmt((Expr)exp1, new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++")), new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("y")), "++")));
		ASTObject stat4A1 = new IfStmt((Expr)exp1, new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++")), new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("y")), "++")));
		ASTObject stat4B = new IfStmt(new NumericTestOpExpr(new VarExpr(new Identifier("x")), "==", new VarExpr(new Identifier("y"))), (Stmt)stat1, new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("y")), "++")));
		ASTObject stat4C = new IfStmt(new NumericTestOpExpr(new VarExpr(new Identifier("x")), "==", new VarExpr(new Identifier("y"))), new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++")), (Stmt)stat2);
		ASTObject stat4D = new IfStmt(new NumericTestOpExpr(new VarExpr(new Identifier("x")), "==", new VarExpr(new Identifier("y"))), new ExprStmt(new PostfixExpr(new VarExpr(new Identifier("x")), "++")), new CodeBody(new Stmts(new Stmt[] {(VarDec)var1})));
		System.out.println(stat1.unparse());
		System.out.println(expr1.unparse());
		System.out.println(expr2.unparse());
		System.out.println(stat3.unparse());
		System.out.println(stat3A.unparse());
		System.out.println(stat4.unparse());
		System.out.println(stat4A.unparse());
		System.out.println(stat4A1.unparse());
		System.out.println(stat4B.unparse());
		System.out.println(stat4C.unparse());
		System.out.println(stat4D.unparse());
	}
}
