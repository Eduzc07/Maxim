WorkerScript.onMessage = function(msg) {
    msg.model.clear()
    if (msg.action == 'Result') {
        for(let i in msg.list){
            var rider = msg.list[i].split(",");
            var data = {
                "position": rider[0],
                "name": rider[1],
                "categoria": rider[2],
                "club": rider[3],
                "home": rider[4],
                "number": rider[5],
                "chronoTime": rider[6],
                "diffTime": rider[7]};
            msg.model.append(data);
        }
        msg.model.sync();       // updates the changes to the list
    }

    if (msg.action == 'CatMenu') {
        for(let i in msg.list){
            var cate = msg.list[i].split(",")
            var data = {
                "key": cate[0],
                "value": parseInt(cate[1])};
            msg.model.append(data);
        }
        msg.model.sync();       // updates the changes to the list
    }
}
