import java.io.*;
import java.util.*;

public class QuickSortStep {

    public static List<String[]> readCSV(String filename) throws IOException {
        List<String[]> data = new ArrayList<>();
        BufferedReader br = new BufferedReader(new FileReader(filename));
        String line;
        while ((line = br.readLine()) != null) {
            data.add(line.split(","));
        }
        br.close();
        return data;
    }

    public static void main(String[] args) throws Exception {
        if (args.length < 3) {
            System.out.println("Usage: java QuickSortStep <filename> <start_row> <end_row>");
            return;
        }

        String filename = args[0];
        int start = Integer.parseInt(args[1])-1;
        int end = Integer.parseInt(args[2])-1;

        List<String[]> data = readCSV(filename);

        List<String[]> subList = new ArrayList<>(data.subList(start, end + 1));

        String outputFilename = "quick_sort_step_" + (start+1) + "_" + (end+1) + ".txt";
        BufferedWriter writer = new BufferedWriter(new FileWriter(outputFilename));

        int[] stepCounter = {0};

        QuickSort.quickSortWithTrace(subList, 0, subList.size() - 1, writer, stepCounter);

        writer.close();
        System.out.println("Step-by-step trace written to " + outputFilename);
    }
}
