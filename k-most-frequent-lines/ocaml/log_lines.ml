open Core.Std

let tuple_less_than a b =
  let a_snd = Tuple.T2.get2 a in
  let b_snd = Tuple.T2.get2 b in
  b_snd - a_snd
;;

let tuple_greater_than a b =
  let a_snd = Tuple.T2.get2 a in
  let b_snd = Tuple.T2.get2 b in
  a_snd - b_snd
;;

let generate_frequency_table file_path =
  let frequency_table = Hashtbl.Poly.create () in
  List.iter 
    ~f:(fun line ->
      let current_count = 
        match Hashtbl.find frequency_table line with
          | None -> 0
          | Some freq -> freq
      in Hashtbl.replace frequency_table ~key:line ~data:(current_count + 1))
    (In_channel.read_lines file_path);
  frequency_table
;;

let generate_heap frequency_table k =
  let k_heap = Hash_heap.Heap.create ~cmp:tuple_greater_than () in

  Hashtbl.keys frequency_table
  |> Sequence.of_list
  |> (fun seq -> Sequence.take seq k)
  |> Sequence.iter
      ~f:(fun line ->
        let freq = Hashtbl.Poly.find_exn frequency_table line in
        Hash_heap.Heap.add k_heap (line, freq);
        Hashtbl.remove frequency_table line);

  Hashtbl.iter frequency_table
    ~f:(fun ~key:line ~data:freq ->
      if freq > Tuple2.get2 (Option.value_exn (Hash_heap.Heap.top k_heap))
      then Hash_heap.Heap.add k_heap (line, freq)
      else ());

  let reversed_heap = Hash_heap.Heap.create ~cmp:tuple_less_than ()
  in Hash_heap.Heap.iter k_heap
  ~f:(fun tuple -> Hash_heap.Heap.add reversed_heap tuple);

  reversed_heap
;;

let print_most_frequent_lines file_path k =
  let table = generate_frequency_table file_path in
  let heap = generate_heap table k in
  let num_pops =
    if k < Hash_heap.Heap.length heap
    then k
    else Hash_heap.Heap.length heap in

  for _i = 1 to num_pops do
    let top = Option.value_exn (Hash_heap.Heap.pop heap) in
    printf "%s : %d\n" (Tuple2.get1 top) (Tuple2.get2 top);
  done
;;

let () =

  let argc = Array.length Sys.argv in

  if argc < 2
  then
    printf "Filename is required."
  else
    let k =
      if argc = 3
      then
        int_of_string Sys.argv.(2)
      else
        10
    in
    print_most_frequent_lines Sys.argv.(1) k
;;
