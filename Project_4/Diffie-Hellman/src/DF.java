import java.io.IOException;
import java.math.BigInteger;
import java.security.SecureRandom;
import java.util.Scanner;

public class DF {
    public static void main(String[] args) throws IOException {
        DF df = new DF();
        df.run();
    }

    public void run() throws IOException {
        BigInteger p = generateBigPrime();
//        BigInteger p = new BigInteger("2352879078719933781639060736715323439598294281697091822572741747863883101469025359832459625617217591245138252003241366094853573286052325819234382885480040783272948035006851188273047");
        System.out.printf("p = %d\n\n", p);
        BigInteger a = generateBigRandom();
//        BigInteger a = new BigInteger("3368446358283588182838945546695157874498600860292397452432368707324274031826631359854027780950067920794826420779496309029982708253043570721998933988130347660592210780312640651367280");
        System.out.printf("a = %d\n\n", a);
        BigInteger g = new BigInteger("5");
        System.out.printf("g = %d\n\n", g);

        BigInteger modded = bigModExp(g, a, p);
        System.out.printf("ga = %d\n\n", modded);

        System.out.println("Enter gb:");
        Scanner scanner = new Scanner(System.in);
        BigInteger gb = new BigInteger(scanner.next());
        System.out.printf("gb = %d\n\n", gb);

        BigInteger secret = bigModExp(gb, a, p);
        System.out.printf("secret = %d\n\n", secret);
    }

    static BigInteger bigModExp(BigInteger base, BigInteger pow, BigInteger m)
    {
        BigInteger res = BigInteger.ONE;
        base = base.mod(m);
        if (base.compareTo(BigInteger.ZERO) == 0)
            return BigInteger.ZERO;

        while (pow.compareTo(BigInteger.ZERO) > 0)
        {
            if (pow.and(BigInteger.ONE).compareTo(BigInteger.ZERO) != 0)
                res = res.multiply(base).mod(m);

            pow = pow.shiftRight(1);
            base = base.multiply(base).mod(m);
        }
        return res;
    }

    public static BigInteger generateBigPrime() {
        SecureRandom random = new SecureRandom();
        BigInteger prime;
        do {
            prime = new BigInteger(600, random);
        } while (prime.bitLength() < 500 || !prime.isProbablePrime(100));
        return prime;
    }

    public static BigInteger generateBigRandom() {
        SecureRandom random = new SecureRandom();
        BigInteger x;
        do {
            x = new BigInteger(600, random);
        } while (x.bitLength() < 500);
        return x;
    }
}
