import re
import matplotlib.pyplot as plt

class mutation_table:
    locations = []
    counts = []

    def __init__(self):
        self.locations = []
        self.counts = []

    def add_data_point(self, loc, count):
        self.locations.append(loc)
        self.counts.append(count)

def get_int(string):
    return int(re.search(r'\d+', string).group())



if __name__ == "__main__":

    inputfile = open("allgenes_mut_moddb_results.txt")
    lines = inputfile.readlines()
    mut_counts = {}

    for line in [li.strip() for li in lines]:
        entry = line.split(' ')

        for mutation in entry[1].split(','):
            if mutation in mut_counts:
                mut_counts[mutation] = mut_counts.get(mutation, 0) + 1
                if mutation == "ORF1a_polyprotein:I300F":
                    print("", end="") # This is here to have a line to selectively break on with GDB
            else:
                mut_counts[mutation] = 1

    gene_tab_dict = {}
    for mut in mut_counts.items():
        details = mut[0].split(':')
        loc = get_int(details[1])
        count = mut[1]

        if details[0] in gene_tab_dict:
            gene_tab_dict[details[0]].add_data_point(loc, count)
        else:
            gene_tab_dict[details[0]] = mutation_table()
            gene_tab_dict[details[0]].add_data_point(loc, count)

    gene_tab_dict_subset = [(gene, gene_tab_dict[gene]) for gene in ["ORF1a_polyprotein",
                                                             "ORF1b_polyprotein",
                                                             "surface_glycoprotein",
                                                             "nucleocapsid_phosphoprotein"]]

    fig = plt.figure()
    fig_loc = 221

    for title, table in gene_tab_dict_subset:

        # ut_frame = pd.DataFrame({"Location": table.locations,
        #                          "Counts": table.counts}
        print(title)
        print(table.locations)
        print(table.counts)
        fig.set_size_inches((10, 10))

        ax = fig.add_subplot(fig_loc)
        ax.scatter(table.locations, table.counts)
        plt.yscale("log")
        ax.set_title(title)
        ax.set_ylabel("Mutation position")

        fig_loc += 1

    plt.show()


