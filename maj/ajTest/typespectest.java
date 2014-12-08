package maj.ajTest;


import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class typespectest {
	public static void main(String[] myargs)  {
		TypeD type1 = new ResolvedTypeD(new MajUncheckedType(new Identifier("int")));
		TypeD type2 = new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))));
		TypeD type3 = new ArrayTypeD(new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new Identifier("int")))));
		TypeD type4 = new ResolvedTypeD(new MajUncheckedType(new Identifier("int")));
		TypeD type5 = new ResolvedTypeD(new MajUncheckedType(new Identifier("int")));
		TypeD type6 = new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(new Identifier("int"))));
		TypeD type7 = new ResolvedTypeD(new MajUncheckedType(new Identifier(new Identifier("Foo"), new Identifier("Bar"))));
		TypeD type8 = new ResolvedTypeD(new MajUncheckedType(type1));
		TypeD type9 = new ResolvedTypeD(new MajUncheckedType(type1));
		TypeD type10 = new ArrayTypeD(new ResolvedTypeD(new MajUncheckedType(type1)));
		TypeD selftyp1 = new ResolvedTypeD(new MajUncheckedType(type4));
		TypeD selftyp2 = new ResolvedTypeD(type4);
		System.out.println(type1.unparse());
		System.out.println(type2.unparse());
		System.out.println(type3.unparse());
		System.out.println(type4.unparse());
		System.out.println(type5.unparse());
		System.out.println(type6.unparse());
		System.out.println(type7.unparse());
		System.out.println(type8.unparse());
		System.out.println(type9.unparse());
		System.out.println(selftyp1.unparse());
		System.out.println(selftyp2.unparse());
	}
}
