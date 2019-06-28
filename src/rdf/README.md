# RDF

## Application development

### RDF extension

```
rule <name> {

left {
    topology {
        ...
    }
    production {    
        ...
    }
    consumption {    
        ...
    }
    actor ... {
        ...
    }
}

right {
    topology {
        ...
    }
    production {    
        ...
    }
    consumption {    
        ...
    }
    actor ... {
        ...
    }
}

}
```

### Controller

A controller decides when and how to apply a transformation rule. It is not implemented yet. Controller can be implemented as follows.

```
main {
   <condition_1> : <rule_x>;
   ...
   <condition_n> : <rule_y>;
}
```


## Getting started

### Build

```
./build -DBUILD_RDF_MODULE=ON
```

### Run

```
./rdf ../test/rdf/cannyshow.df
```

### Commands 

```
graph               display the graph.
h                   display help menu.
latency             compute the lateny of the graph.
program             display the RDF program.
rules               display list of rules.
run                 run the RDF program.
schedule            compute an schedule for the graph.
```

## Logging and plotting

If the `logging` variable in the `parameter` section of the dataflow graph is set to `true`, logs are directed to the console, otherwise logs are written into files in `~/Documents/df/logs`.

Logs should be concatenated in a `.log` file and the file should be passed as argument to the `csvCreate.py` script. This script extracts the start time, end time, and latency of each iteration, in adition to the time that the actors have spent in total for execution. The result is written to a `.csv` file. These files should be passed to `plor.r` script in the `plots` directory to generate the resulting plots in `.pdf` format.
