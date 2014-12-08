package maj.ajTest;


import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class MAJExample1Test {
	public static void main(String[] myargs)  {
		AspectDec aspect1 = new AspectDec(null, "foo", null, null, null, null, new Decs(new Dec[] {null}));
		ResolvedTypeD interface1 = new ResolvedTypeD(new MajUncheckedType(new Identifier("InterfaceOne")));
		Identifier interId = new Identifier("InterfaceTwo");
		ResolvedTypeD interface2 = new ResolvedTypeD(interId);
		aspect1.addInterface(interface1);
		aspect1.addInterface(interface2);
		Identifier aspectName = new Identifier("OriginalClass_Aspect");
		CallPcd pct1 = new CallPcd(new MethodPattern(null, null, new AnyTypeName(), new SimpleName(new Identifier("*")), new FormalsPattern(new GenTypeNames(new GenTypeName[] {new EllipsesFakeTypeName()})), null));
		NotPcd pct2 = new NotPcd(new WithinPcd(new SimpleTypeName(new MajUncheckedType("OriginalClass_Aspect"))));
		NotPcd pct3 = new NotPcd(new CFlowPcd(new WithinPcd(new SimpleTypeName(new MajUncheckedType("OriginalMethod")))));
		TargetPcd pct4 = new TargetPcd(new Identifier("OriginalMethod"));
		AroundAdviceDec adv1 = new AroundAdviceDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("Object"))), new Formals(new FormalDec[] {}), null, new AndPcd(new AndPcd(new AndPcd(pct1, pct2), pct3), pct4), new CodeBody(new Stmts(new Stmt[] {new TryCatchStmt(new CodeBody(new Stmts(new Stmt[] {new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("Method"))))), new Identifier("meth"), new CallExpr(new CallExpr(null, "ep.getClass"), "getMethod", new Exprs(new Expr[] {new CallExpr(new CallExpr(null, "thisJointPoint.getSignature"), "getName"), new CallExpr(new CallExpr(null, "thisJointPoint.getSignature"), "getParameterTypes")}))), new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("Object"))))), new Identifier("result"), new CallExpr(null, "meth.invoke", new Exprs(new Expr[] {new VarExpr(new Identifier("ep")), new CallExpr(null, "thisJoinPoint.getArgs")}))), new ReturnStmt(new VarExpr(new Identifier("result")))})), new CatchClauses(new CatchClause[] {new CatchClause(new FormalDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("Exception"))), "e"), new CodeBody(new Stmts(new Stmt[] {})))}))})));
		AndPcd pct5 = new AndPcd(new AndPcd(new TargetPcd(new Identifier("OriginalMethod")), new NotPcd(new WithinPcd(new SimpleTypeName(new MajUncheckedType("OriginalClass_Aspect"))))), new NotPcd(new CFlowPcd(new WithinPcd(new SimpleTypeName(new MajUncheckedType("OriginalMethod"))))));
		VarDec dec1 = new VarDec(new Modifiers(2), new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("InterfaceName"))))), new Identifier("ep"));
		Identifier id1 = new Identifier("JndiName");
		MethodDec meth1 = new MethodDec(null, null, "OriginalClass_Aspect", new Formals(new FormalDec[] {}), null, new CodeBody(new Stmts(new Stmt[] {new TryCatchStmt(new CodeBody(new Stmts(new Stmt[] {new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("OriginalMethodHome"))))), new Identifier("sh")), new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier(new Identifier(new Identifier("javax"), new Identifier("naming")), new Identifier("InitialContext")))))), new Identifier("initContext"), new NewInstanceExpr(new ResolvedTypeD(new MajUncheckedType(new Identifier(new Identifier(new Identifier("javax"), new Identifier("naming")), new Identifier("InitialContext")))), null)), new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("String"))))), new Identifier("JNDIName"), id1), new VarDec(null, new ResolvedTypeD(new MajUncheckedType(new ResolvedTypeD(new MajUncheckedType(new Identifier("Object"))))), new Identifier("obj"), new CallExpr(null, "initContext.lookup", new Exprs(new Expr[] {new VarExpr(new Identifier("JNDIName"))})))})), new CatchClauses(new CatchClause[] {new CatchClause(new FormalDec(null, new ResolvedTypeD(new MajUncheckedType(new Identifier("Exception"))), "e"), new CodeBody(new Stmts(new Stmt[] {})))}))})));
		aspect1.addMember(meth1);
		aspect1.addMember(adv1);
		System.out.println(aspect1.unparse());
	}
}
