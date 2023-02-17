import java.math.BigInteger;
import java.security.SecureRandom;
import java.util.Scanner;

public class Rsa {

    public static void main(String[] args) {
        Rsa rsa = new Rsa();
        rsa.run();
    }

    public void run() {
        BigInteger e = new BigInteger("65537");
        BigInteger p = generatePrime(512);
        BigInteger q = generatePrime(512);
        BigInteger phi = p.subtract(BigInteger.ONE).multiply(q.subtract(BigInteger.ONE));

        while(!areRelativelyPrime(phi, e)) {
            p = generatePrime(512);
            q = generatePrime(512);
            phi = p.subtract(BigInteger.ONE).multiply(q.subtract(BigInteger.ONE));
        }

        BigInteger n = p.multiply(q);
        BigInteger d = extendedEuclidean(e, phi)[1];
        if (d.compareTo(BigInteger.ZERO) < 0) {
            d = d.add(phi);
        }

        System.out.printf("p = %d\n", p);
        System.out.printf("q = %d\n", q);
        System.out.printf("n = %d\n", n);
        System.out.printf("d = %d\n", d);

        Scanner scanner = new Scanner(System.in);
        System.out.print("\nEnter value to encrypt: ");
        BigInteger toEncrypt = new BigInteger(scanner.next());
        System.out.printf("You entered %d\n", toEncrypt);
        BigInteger encrypted = bigModExp(toEncrypt, e, n);
        System.out.printf("encrypted text = %d\n", encrypted);

        System.out.print("\nEnter value to decrypt: ");
        BigInteger toDecrypt = new BigInteger(scanner.next());
        System.out.printf("You entered %d\n", toDecrypt);
        BigInteger decrypted = bigModExp(toDecrypt, d, n);
        System.out.printf("decrypted text = %d\n", decrypted);
    }

    public static boolean areRelativelyPrime(BigInteger a, BigInteger b) {
        return a.gcd(b).equals(BigInteger.ONE);
    }

    public static BigInteger[] extendedEuclidean(BigInteger a, BigInteger b) {
        if (b.equals(BigInteger.ZERO)) {
            return new BigInteger[] { a, BigInteger.ONE, BigInteger.ZERO };
        } else {
            BigInteger[] values = extendedEuclidean(b, a.mod(b));
            BigInteger gcd = values[0];
            BigInteger x = values[2];
            BigInteger y = values[1].subtract(a.divide(b).multiply(values[2]));
            return new BigInteger[] { gcd, x, y };
        }
    }

    public static BigInteger bigModExp(BigInteger base, BigInteger pow, BigInteger m)
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

    public static BigInteger generatePrime(int bitLength) {
        SecureRandom random = new SecureRandom();
        BigInteger prime = BigInteger.probablePrime(bitLength, random);
        while (!prime.isProbablePrime(100)) {
            prime = BigInteger.probablePrime(bitLength, random);
        }
        return prime;
    }
}
