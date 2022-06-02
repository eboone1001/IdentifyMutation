# IdentifyMutation
A program for identifying mutations between a reference and a user supplied query.

### create a Fasta formatted database of SARS-CoV-2 proteins
To produce a fasta formatted database of SARS-CoV-2 Proteins we produced the tool ./annotate.exe which inputs .gff annotations from the NCBI website, and a reference genome, and outputs to a fasta file.
This specific command was used to produce the file 'protein_database.fasta' for our work: ./annotate.exe -a GCF_009858895.2_ASM985889v3_genomic.gff -r sars_cov2_ref.fasta -o protein_database.fasta -b -p

This tool also plots the genome with annotations in the html file genome.html and can plot the genome with annotations and mutations after later steps with

./annotate.exe -a GCF_009858895.2_ASM985889v3_genomic.gff -r sars_cov2_ref.fasta -i test -m allgenes_mut_moddb_results.txt

### Download viral genomic data
<p>First, we went to NCBI Virus site as directed, then filtered for our chosen continent (Oceania) and then downloaded **Just the accession numbers** with version. On our local machine, we then selected 100 random accessions by using the "shuf" commnad as follows:
> shuf -n 100 accessions.acc > picked\_accessions.txt
</p>
<p>Next, we downloaded and installed the ncbi\_datasets\_cli package though conda.  We downloaded the actual fasta data via the command:
> datasets download virus genome accession --inputfile picked\_accessions.txt
Which downloaded a zipped directory containing the 100 genomes we selected in a singe FASTA file (.faa)
</p>

### Blast sequences against the database
<p>In order to blast our randomly selected sequences against the reference genome, we downloaded and installed the BLAST+ command line program.

</p>
