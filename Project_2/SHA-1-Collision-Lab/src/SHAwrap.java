import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;

public class SHAwrap {
    private final static String SHA_1 = "SHA-1";
    private static MessageDigest shaEncoder = null;

    public int hash(byte[] toHash, int numBits) {
        MessageDigest encoder = getSHA1Encoder();
        byte[] hashed = encoder.digest(toHash);
        return truncate(hashed, numBits);
    }

    public MessageDigest getSHA1Encoder() {
        if (shaEncoder != null) return shaEncoder;
        try {
            shaEncoder = MessageDigest.getInstance(SHA_1);
        } catch (java.security.NoSuchAlgorithmException e) {
            e.printStackTrace();
            System.exit(0);
        }
        return shaEncoder;
    }

    public int bitExtracted(int number, int numBits)
    {
        return (((1 << numBits) - 1) & number);
    }

    public int truncate(byte[] bytes, int numBits) {
        int value = 0, numItr = numBits/8;
        if (numBits%4 != 0) numItr++;
        for (int i = bytes.length-1-numItr; i < bytes.length; i++) {
            byte b = bytes[i];
            value = (value << 8) + (b & 0xFF);
        }
        return bitExtracted(value,numBits);
    }

    public byte[] generateRandBytes(int length) {
        byte[] bytes = new byte[length];
        try {
            SecureRandom.getInstanceStrong().nextBytes(bytes);
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        return bytes;
    }
}
