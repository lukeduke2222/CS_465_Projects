public class PreImageTest {
    int toMatch;
    int numHashes;
    SHAwrap encoder;

    public int run(int numBits, int rounds, int roundNum) {
        reset();
        encoder = new SHAwrap();
        byte[] toHash = encoder.generateRandBytes(20);
        toMatch = encoder.hash(toHash, numBits);
        loopHashes(numBits);

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
