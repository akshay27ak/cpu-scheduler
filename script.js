function toggleFields() {
    var algorithm = document.getElementById("algorithm").value;
    var priorityField = document.getElementById("priority-field");
    var quantumField = document.getElementById("quantum-field");
  
    // Ensures default fields are always visible
    document.getElementById("process-id").style.display = "block";
    document.getElementById("arrival-time").style.display = "block";
    document.getElementById("burst-time").style.display = "block";
  
    // Hiding algorithm-specific fields initially
    priorityField.style.display = "none";
    quantumField.style.display = "none";
  
    // Showing fields based on selected algorithm
    if (algorithm === "PS" || algorithm === "MLQ" || algorithm === "AUTO") {
      priorityField.style.display = "flex";
    }
    if (algorithm === "RR" || algorithm === "AUTO" || algorithm === "MLQ" ) {
      quantumField.style.display = "flex";
    }
  }
  