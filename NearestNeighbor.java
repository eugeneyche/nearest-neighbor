import java.io.*;
import java.util.*;

public class NearestNeighbor
{
    private volatile Vector<Bundle> trainSet;
    private volatile Vector<Bundle> testSet;

    public static class Bundle
    {
        private int classifier;
        private Vector<Integer> vector;

        public Bundle(int classifier, Vector<Integer> vector)
        {
            this.classifier = classifier;
            this.vector = vector;
        }

        public double distanceTo(Vector<Integer> vector)
        {
            if (vector.size() != this.vector.size())
                return -1;
            double sum = 0;
            for (int i = 0; i < vector.size(); i++)
            {
                double dist = vector.get(i) - this.vector.get(i);
                sum += dist * dist;
            }
            return sum;
        }

        public int getClassifier()
        {
            return classifier;
        }

        public Vector<Integer> getVector()
        {
            return vector;
        }
    }

    public static int nn1(Bundle test, Vector<Bundle> trainSet)
    {
        double minDistance = test.distanceTo(trainSet.get(0).getVector());
        int min = 0;
        for (int j = 1; j < trainSet.size(); j++)
        {
            double lDistance = test.distanceTo(trainSet.get(j).getVector());
            if (lDistance < minDistance)
            {
                minDistance = lDistance;
                min = j;
            }
        }
        return min;
    }

    public static int readBytes(int numBytes, InputStream in) throws IOException
    {
        int result = 0;
        for (int i = 0; i < numBytes; i++)
        {
            result = (result << 8) + in.read();
        }
        return result;
    }

    public static boolean loadSet(String pathToLabels, String pathToVectors, Vector<Bundle> set) throws IOException
    {
        boolean success = false;
        InputStream labelInput = ClassLoader.class.getResourceAsStream(pathToLabels);
        InputStream vectorInput = ClassLoader.class.getResourceAsStream(pathToVectors);
        if (readBytes(4, labelInput) == 2049 && readBytes(4, vectorInput) == 2051)
        {
            int numVectors = -1;
            if ((numVectors = readBytes(4, labelInput)) == readBytes(4, vectorInput))
            {
                int size = readBytes(4, vectorInput) * readBytes(4, vectorInput);
                for (int i = 0; i < numVectors; i++)
                {
                    int label = labelInput.read();
                    Vector<Integer> vector = new Vector<Integer>();
                    for (int j = 0; j < size; j++)
                    {
                        vector.add(vectorInput.read());   
                    }
                    set.add(new Bundle(label, vector));
                }
                success = true;
            }
        }
        labelInput.close();
        vectorInput.close();
        return success;
    }
}
