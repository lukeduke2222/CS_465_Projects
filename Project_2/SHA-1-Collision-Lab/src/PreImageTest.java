import java.util.HashSet;
import java.util.Set;

public class PreImageTest {
    int toMatch;
    int numHashes;
    SHAwrap encoder;

    public int run(int numBits, int rounds, int roundNum) {
        reset();
        encoder = new SHAwrap();
        byte[] toHash = encoder.generateRandBytes(20);
        toMatch = encoder.hash(toHash, numBits);
//        System.out.printf("Hashing to find: 0x%06x on %d bits\n", toMatch, numBits);
//        System.out.printf("Expecting to find within %d hashes\n", (int)Math.pow(2, numBits));
        loopHashes(numBits);
//        System.out.printf("PreImaged hash found: 0x%06x on the %dth hash on %d bits\n\n", toMatch, numHashes, numBits);
        String filename = "PreImageTest-"+numBits+"Bit-"+rounds+"Rounds.txt";
        String data = String.format("Trial #%02d:\t%dth hash\n", roundNum, numHashes);
        TestLog.appendToFile(filename, data);
        return numHashes;
    }

    public void loopHashes(int numBits) {
        while (true) {
            byte[] toHash = encoder.generateRandBytes(20);
            int hash = encoder.hash(toHash, numBits);
            numHashes++;
            if (hash == toMatch) break;
        }
    }

    public void reset() {
        numHashes = 0;
    }
}
