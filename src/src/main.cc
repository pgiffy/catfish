#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <iostream>
#include <ctime>
#include <cassert>
#include <sstream>

#include "config.h"
#include "catfish.h"
#include "gtf.h"
#include "genome.h"
#include "nested_graph.h"
#include "sgraph_compare.h"

using namespace std;

int assemble_gtf(const string &file);
int assemble_sgr(const string &file);

int main(int argc, const char **argv)
{
	srand(time(0));
	parse_arguments(argc, argv);
	print_parameters();

	if(input_file == "" && output_file != "")
	{
		splice_graph sg;
		sg.simulate(simulation_num_vertices, simulation_num_edges, simulation_max_edge_weight);
		sg.write(output_file);
		return 0;
	}

	if(input_file != "")
	{
		string s = input_file.substr(input_file.size() - 3, 3);
		if(s == "gtf") assemble_gtf(input_file);
		if(s == "sgr") assemble_sgr(input_file);
	}
	return 0;
}

int assemble_gtf(const string &file)
{
	genome g(file);

	ofstream fout;
	if(output_file != "") fout.open(output_file);

	for(int i = 0; i < g.genes.size(); i++)
	{
		gtf gg(g.genes[i]);

		if(gg.transcripts.size() < min_gtf_transcripts_num) continue;

		string name = gg.get_gene_id();

		if(fixed_gene_name != "" && name != fixed_gene_name) continue;

		splice_graph gr;
		gg.build_splice_graph(gr);

		catfish sc(name, gr);
		sc.assemble();

		if(output_file == "") continue;
		gg.output_gtf(fout, sc.paths, algo);
	}

	if(output_file != "") fout.close();
	return 0;
}

int assemble_sgr(const string &file)
{
	splice_graph sg;
	sg.build(file);

	catfish sc("shao", sg);
	sc.assemble();

	return 0;
}