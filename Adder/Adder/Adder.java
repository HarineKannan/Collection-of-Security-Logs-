import java.util.Arrays;

public class Adder {
    // Load the native library
    static {
        System.load("D:\\Projects\\Adder JNI\\Adder\\Adder\\Adder.dll");
    }

    // Native method declaration to retrieve security logs
    //private native LogEntry[] retrieveSecurityLogs();
    public native int  retrieveSecurityLogs();

    // Main method to test the native method
    public static void main(String[] args) {
        Adder adder = new Adder();
        adder.retrieveSecurityLogs();
    }
        /*LogEntry[] logs = adder.retrieveSecurityLogs();
        
        // Print the retrieved security logs
        for (LogEntry log : logs) {
            System.out.println("TimeGenerated: " + log.getTimeGenerated() + 
                               ", EventCode: " + log.getEventCode() + 
                               ", EventType: " + log.getEventType());
        }
    }

    // Inner class representing a log entry
    public static class LogEntry {
        private final String timeGenerated;
        private final int eventCode;
        private final int eventType;

        public LogEntry(String timeGenerated, int eventCode, int eventType) {
            this.timeGenerated = timeGenerated;
            this.eventCode = eventCode;
            this.eventType = eventType;
        }

        public String getTimeGenerated() {
            return timeGenerated;
        }

        public int getEventCode() {
            return eventCode;
        }

        public int getEventType() {
            return eventType;
        }
    }*/
}
