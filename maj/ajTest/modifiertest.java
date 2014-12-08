package maj.ajTest;


import java.io.*;
import org.aspectj.compiler.base.*;
import org.aspectj.compiler.base.ast.*;
import org.aspectj.compiler.base.cst.*;
import org.aspectj.compiler.crosscuts.ast.*;

public class modifiertest {
	public static void main(String[] myargs)  {
		Modifiers mods1 = new Modifiers(9);
		Modifiers mods2 = new Modifiers(9);
		Modifiers mods3 = new Modifiers(9);
		Modifiers mods4 = new Modifiers(9);
		Modifiers odd = null;
		Modifiers selfmods1 = mods4;
		Modifiers selfmods2 = mods4;
		System.out.println(mods1.unparse());
		System.out.println(mods2.unparse());
		System.out.println(mods3.unparse());
		System.out.println(mods4.unparse());
		System.out.println(odd.unparse());
		System.out.println(selfmods1.unparse());
		System.out.println(selfmods2.unparse());
	}
}
