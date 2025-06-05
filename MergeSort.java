
import java.io.*;
import java.util.*;

public class MergeSort {

    static class Data {

        int number;
        String str;

        Data(int number, String str) {
            this.number = number;
            this.str = str;
        }
    }

    static class SegmentReader implements Comparable<SegmentReader> {

        BufferedReader reader;
        Data current;
        boolean hasNext;

        public SegmentReader(BufferedReader reader) {
            this.reader = reader;
            advance();
        }

        void advance() {
            try {
                String line = reader.readLine();
                if (line == null) {
                    hasNext = false;
                    current = null;
                    reader.close();
                } else {
                    String[] parts = line.split(",", 2);
                    int num = Integer.parseInt(parts[0]);
                    current = new Data(num, parts.length > 1 ? parts[1] : "");
                    hasNext = true;
                }
            } catch (IOException e) {
                hasNext = false;
                current = null;
            }
        }

        @Override
        public int compareTo(SegmentReader other) {
            return Integer.compare(this.current.number, other.current.number);
        }
    }

    public static void main(String[] args) {
        if (args.length != 3) {
            System.err.println("Usage: java MergeSort <input_file> <output_file> <segment_size>");
            System.exit(1);
        }

        String inputFile = args[0];
        String outputFile = args[1];
        int segmentSize = Integer.parseInt(args[2]);

        int totalRows = 0;
        try (BufferedReader br = new BufferedReader(new FileReader(inputFile))) {
            while (br.readLine() != null) {
                totalRows++;
            }
        } catch (IOException e) {
            System.err.println("Error reading input file: " + e.getMessage());
            System.exit(1);
        }

        int segments = (totalRows + segmentSize - 1) / segmentSize;
        List<Process> processes = new ArrayList<>();

        for (int i = 0; i < segments; i++) {
            int start = i * segmentSize + 1;
            int end = Math.min((i + 1) * segmentSize, totalRows);
            ProcessBuilder pb = new ProcessBuilder("java", "MergeSortStep", inputFile, String.valueOf(start), String.valueOf(end));
            try {
                Process p = pb.start();
                processes.add(p);
            } catch (IOException e) {
                System.err.println("Error starting MergeSortStep process: " + e.getMessage());
            }
        }

        for (Process p : processes) {
            try {
                p.waitFor();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                System.err.println("Process interrupted: " + e.getMessage());
            }
        }

        PriorityQueue<SegmentReader> queue = new PriorityQueue<>();
        List<SegmentReader> readers = new ArrayList<>();

        for (int i = 0; i < segments; i++) {
            int start = i * segmentSize + 1;
            int end = Math.min((i + 1) * segmentSize, totalRows);
            String segmentFile = "sorted_segment_" + start + "_" + end + ".csv";
            try {
                BufferedReader br = new BufferedReader(new FileReader(segmentFile));
                SegmentReader reader = new SegmentReader(br);
                if (reader.hasNext) {
                    queue.add(reader);
                    readers.add(reader);
                }
            } catch (IOException e) {
                System.err.println("Error reading sorted segment: " + e.getMessage());
            }
        }

        try (PrintWriter pw = new PrintWriter(outputFile)) {
            while (!queue.isEmpty()) {
                SegmentReader reader = queue.poll();
                Data data = reader.current;
                pw.println(data.number + "," + data.str);
                reader.advance();
                if (reader.hasNext) {
                    queue.add(reader);
                }
            }
        } catch (FileNotFoundException e) {
            System.err.println("Error writing output file: " + e.getMessage());
        }

        for (SegmentReader reader : readers) {
            try {
                if (reader.reader != null) {
                    reader.reader.close();
                }
            } catch (IOException e) {
                System.err.println("Error closing reader: " + e.getMessage());
            }
        }
    }
}
