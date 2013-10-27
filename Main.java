import javax.swing.*;
import java.io.*;
import java.util.*;
import java.awt.image.BufferedImage;
import java.awt.FlowLayout;

public class Main
{
    public static final String PATH_TO_TRAIN_VECTORS = "/data/train_vectors";
    public static final String PATH_TO_TRAIN_LABELS = "/data/train_labels";

    public static final String PATH_TO_TEST_VECTORS = "/data/test_vectors";
    public static final String PATH_TO_TEST_LABELS = "/data/test_labels";

    public static final int IMAGE_WIDTH = 28;
    public static final int IMAGE_HEIGHT = 28;
    
    public abstract static class Command
    {
        public abstract void run(String[] args);
    }

    public abstract class TestThread extends Thread 
    {
        public int set;
        public int nn;

        public TestThread(int set)
        {
            this.set = set;
        }

        public void run()
        {
            runningThreads++;
            nn = test();
            runningThreads--;

        }

        public abstract int test();
    }

    private HashMap<String, Command> commands;
    private volatile boolean isRunning = true;
    private volatile int runningThreads = 0;
    private Vector<NearestNeighbor.Bundle> trainSet;
    private Vector<NearestNeighbor.Bundle> testSet;

    public Main initialize() throws IOException
    {
        commands = new HashMap<String, Command>();
        trainSet = new Vector<NearestNeighbor.Bundle>();
        testSet = new Vector<NearestNeighbor.Bundle>();
        System.out.println("Loading Train Set...");
        if (NearestNeighbor.loadSet(PATH_TO_TRAIN_LABELS, PATH_TO_TRAIN_VECTORS, trainSet)) 
            System.out.println("Done.");
        else
            System.out.println("Failed to load Train Set.");
        System.out.println("Loading Test Set...");
        if (NearestNeighbor.loadSet(PATH_TO_TEST_LABELS, PATH_TO_TEST_VECTORS, testSet)) 
            System.out.println("Done.");
        else
            System.out.println("Failed to load Test Set.");
        commands.put("show", new Command()
        {
            public void run(String[] args)
            {
                NearestNeighbor.Bundle toShow = null;
                JFrame frame = new JFrame();
                frame.setLayout(new FlowLayout(FlowLayout.LEFT, 10, 10));
                for (int pair = 0; pair < args.length; pair += 2) {
                    if (args[pair].equals("train"))
                    {
                        toShow = trainSet.get(Integer.parseInt(args[pair + 1]));
                    }
                    if (args[pair].equals("test"))
                    {
                        toShow = testSet.get(Integer.parseInt(args[pair + 1]));
                    }
                    if (toShow != null)
                    {
                        BufferedImage image = new BufferedImage(IMAGE_WIDTH, IMAGE_HEIGHT, BufferedImage.TYPE_INT_RGB);
                        for (int i = 0; i < toShow.getVector().size(); i++)
                        {
                            int grayscale = 255 - toShow.getVector().get(i);
                            int rgb = (((grayscale << 8) + grayscale) << 8) + grayscale;
                            image.setRGB(i % IMAGE_WIDTH, i / IMAGE_WIDTH, rgb);
                        }
                        ImageIcon imageIcon = new ImageIcon(image, new String());
                        JPanel panel = new JPanel();
                        panel.add(new JLabel(imageIcon));
                        panel.add(new JLabel(String.valueOf(toShow.getClassifier())));
                        frame.add(panel);
                    }
                    frame.pack();
                    frame.setVisible(true);
                }
            }
        });
        commands.put("nn1", new Command()
        {
            public void run(String[] args)
            {
                for (int i = 0; i < args.length; i++)
                {
                    int testCase = Integer.parseInt(args[i]);
                    System.out.print(testCase + " => ");
                    int nn = NearestNeighbor.nn1(testSet.get(testCase), trainSet);
                    System.out.println(nn);
                }
            }
        });
        commands.put("test", new Command()
        {
            public void run(String[] args)
            {
                if (args[0].equals("nn1"))
                {
                    ArrayList<TestThread> tests = new ArrayList<TestThread>();
                    for (int i = 0; i < trainSet.size(); i += 2)
                    {
                        TestThread test1 = new TestThread(i)
                        {
                            public int test()
                            {
                                int nn = NearestNeighbor.nn1(testSet.get(set), trainSet);
                                if (testSet.get(set).getClassifier() == trainSet.get(nn).getClassifier())
                                {
                                    System.out.println(set + " => " + nn);
                                }
                                else
                                {
                                    System.out.println(set + " =x " + nn);
                                }
                                return nn;
                            }
                        };
                        tests.add(test1);
                        test1.start();
                        if (i + 1 < trainSet.size())
                        {
                            TestThread test2 = new TestThread(i + 1)
                            {
                                public int test()
                                {
                                    int nn = NearestNeighbor.nn1(testSet.get(set), trainSet);
                                    if (testSet.get(set).getClassifier() == trainSet.get(nn).getClassifier())
                                    {
                                        System.out.println(set + " => " + nn);
                                    }
                                    else
                                    {
                                        System.out.println(set + " =x " + nn);
                                    }
                                    return nn;
                                }
                            };
                            tests.add(test2);
                            test2.start();
                        }
                        waitForThreads();
                    }
                    waitForThreads();
                    int count = 0;
                    for (TestThread test : tests)
                    {
                        if (testSet.get(test.set).getClassifier() == trainSet.get(test.nn).getClassifier())
                        {
                            count++;
                        }
                    }
                    System.out.println((double)count / trainSet.size() + "% accuracy");
                }
            }
        });
        commands.put("quit", new Command()
        {
            public void run(String[] args)
            {
                isRunning = false;
            }
        });
        return this;
    }

    public void start()
    {
        Scanner in = new Scanner(System.in);
        while (isRunning)
        {
            System.out.print(">>> ");
            String[] toParse = in.nextLine().split(" ");
            if (toParse.length > 0)
            {
                String command = toParse[0];
                String[] args = Arrays.copyOfRange(toParse, 1, toParse.length);
                try
                {
                    commands.get(command).run(args);
                }
                catch (Exception e)
                {
                    e.printStackTrace();
                }
            }
            System.out.println(isRunning);
        }
    }

    public void waitForThreads()
    {
        while (runningThreads > 0) {}
    }
    
    public static void main(String[] args) throws IOException
    {
        new Main().initialize().start();
    }
}
