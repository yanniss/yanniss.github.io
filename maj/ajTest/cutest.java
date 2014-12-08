package maj.ajTest;


import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class cutest {
	public static void main(String[] myargs)  {
		CompilationUnit cu1 = new MajCompilationUnit(new MajPackageExpr(new Identifier(new Identifier("blah"), new Identifier("baz"))), new Imports(new Import[] {new Import(new Identifier(new Identifier("import1"), new Identifier("*")))}), new Decs(new Dec[] {new ClassDec(null, "foo", null, null, new Decs(new Dec[] {null}))}));
		CompilationUnit cu2 = new MajCompilationUnit(null, new Imports(new Import(new Identifier(new Identifier(new Identifier("java"), new Identifier("lang")), new Identifier("*"))), null), new Decs(new Dec[] {new ClassDec(null, "foo", null, null, new Decs(new Dec[] {null}))}));
		CompilationUnit cu3 = new MajCompilationUnit(null, new Imports(new Import(new Identifier(new Identifier(new Identifier("java"), new Identifier("lang")), new Identifier("*"))), null), new Decs(new Dec[] {new ClassDec(null, "foo1", null, null, new Decs(new Dec[] {null})), new ClassDec(null, "foo2", null, null, new Decs(new Dec[] {null}))}));
		System.out.println(cu1.unparse());
		System.out.println(cu2.unparse());
		System.out.println(cu3.unparse());
	}
}
