
# Introduction

Kubernetes is a software system that allows you to easily deploy and manage containerized applications on top of it.

It abstracts away the underlying infrastructure and, by doing so, simplifies development, deployment, and management for both development and the operations teams.



The system is composed of a master node and any number of worker nodes.

When the developer submits a list of apps to the master, Kubernetes deploys them to the cluster of worker nodes.

What node a component lands on doesn’t matter—neither to the developer nor to the system administrator.

The developer can specify that certain apps must run together and Kubernetes will deploy them on the same worker node.

Others will be spread around the cluster, but they can talk to each other in the same way, regardless of where they’re deployed.

Kubernetes can be thought of as an operating system for the cluster.

It relieves application developers from having to implement certain infrastructure-related services into their apps; instead they rely on Kubernetes to provide these services. This includes things such as service discovery, scaling, load-balancing, self-healing, and even leader election.

Application developers can therefore focus on implementing the actual features of the applications and not waste time figuring out how to integrate them with the infrastructure.

Kubernetes will run your containerized app somewhere in the cluster, provide information to its components on how to find each other, and keep all of them running.

Because your application doesn’t care which node it’s running on, Kubernetes can relocate the app at any time, and by mixing and matching apps, achieve far better resource utilization than is possible with manual scheduling.







# Architecture

At the hardware level, a Kubernetes cluster is composed of many nodes which can be split into two types:

- The master node, which hosts the Kubernetes Control Plane that controls and manages the whole Kubernetes system
- Worker nodes that run the actual applications you deploy

The Control Plane is what controls the cluster and makes it function. It consists of multiple components that can run on a single master node or be split across multiple nodes and replicated to ensure high availability. These components are:

- The Kubernetes API Server, which you and the other Control Plane components communicate with
- The Scheduler, which schedules your apps (assigns a worker node to each deployable component of your application)
- The Controller Manager, which performs cluster-level functions, such as replicating components, keeping track of worker nodes, handling node failures, and so on
- etcd, a reliable distributed data store that persistently stores the cluster configuration.

The components of the Control Plane hold and control the state of the cluster, but they don’t run your applications. This is done by the (worker) nodes.

The worker nodes are the machines that run your containerized applications. The task of running, monitoring, and providing services to your applications is done by the following components:

- Docker, rkt, or another container runtime, which runs your containers
- The Kubelet, which talks to the API server and manages containers on its node
- The Kubernetes Service Proxy (kube-proxy), which load-balances network traffic between application components



To run an application in Kubernetes, you first need to package it up into one or more container images, push those images to an image registry, and then post a description of your app to the Kubernetes API server.

The description includes information such as the container image or images that contain your application components, how those components are related to each other, and which ones need to be run co-located (together on the same node) and which don’t.

For each component, you can also specify how many copies (or replicas) you want to run. Additionally, the description also includes which of those components provide a service to either internal or external clients and should be exposed through a single IP address and made discoverable to the other components.

When the API server processes your app’s description, the Scheduler schedules the specified groups of containers onto the available worker nodes based on computational resources required by each group and the unallocated resources on each node at that moment.

The Kubelet on those nodes then instructs the Container Runtime (Docker, for example) to pull the required container images and run the containers.

Once the application is running, Kubernetes continuously makes sure that the deployed
state of the application always matches the description you provided.

If one of those instances stops working
properly, like when its process crashes or when it stops responding, Kubernetes will
restart it automatically.

Similarly, if a whole worker node dies or becomes inaccessible, Kubernetes will
select new nodes for all the containers that were running on the node and run them
on the newly selected nodes.



# Labels

Organizing pods and all other Kubernetes objects is done through labels.

Labels are a simple feature for organizing  Kubernetes resources.

A label is an arbitrary key-value pair you
attach to a resource, which is then utilized when selecting resources using label selectors.

A resource can have more than one label, as long as the keys of those labels are
unique within that resource.

You usually attach labels to resources when you create
them, but you can also add additional labels or even modify the values of existing
labels later without having to recreate the resource.

Labels can also be added to and modified on existing pods.

Label selectors allow you to select a subset of pods tagged with certain labels and perform an operation on those pods. A label selector is a criterion, which filters resources based on whether they include a certain label with a certain value.

A label selector can select resources based on whether the resource

- Contains (or doesn’t contain) a label with a certain key
- Contains a label with a certain key and value
- Contains a label with a certain key, but with a value not equal to the one you specify


```bash
# list pods and labels
$ kubectl get po --show-labels

# list pods and specific labels
$ kubectl get po -L creation_method,env

# list pods with label creation_method=manual
$ kubectl get po -l creation_method=manual

# list all pods with an env label
$ kubectl get po -l env

# list all pods without an env label
$ kubectl get po -l '!env'

# add label
$ kubectl label po kubia-manual creation_method=manual

# change existing label
$ kubectl label po kubia-manual-v2 env=debug --overwrite
```

- creation_method!=manual to select pods with the creation_method label with
any value other than manual
- env in (prod,devel) to select pods with the env label set to either prod or
devel
- env notin (prod,devel) to select pods with the env label set to any value other
than prod or devel

A selector can also include multiple comma-separated criteria. Resources need to
match all of them to match the selector.

Label selectors aren’t useful only for listing pods, but also for performing actions
on a subset of all pods.



# Annotations



# Namespaces



# Health Checks

When you create unmanaged pods, a cluster node is selected to run the pod and then its containers are run on that node. Kubernetes then monitors those containers and automatically restarts them if they fail. But if the whole node
fails, the pods on the node are lost and will not be replaced with new ones.







# Replication Controller





# ReplicaSets

```bash
# list replica sets
$ kubectl get rs

# examine replica set
$ kubectl describe rs

# delete replica set
$ kubectl delete rs RS_NAME
```





# DaemonSet

DaemonSets run only a single pod replica on each node, whereas ReplicaSets
scatter them around the whole cluster randomly.

To run a pod on all cluster nodes, you create a DaemonSet object, which is much
like a ReplicationController or a ReplicaSet, except that pods created by a Daemon-
Set already have a target node specified and skip the Kubernetes Scheduler. They
aren’t scattered around the cluster randomly.

If a node goes down, the DaemonSet doesn’t cause the pod to be created elsewhere.
But when a new node is added to the cluster, the DaemonSet immediately
deploys a new pod instance to it. It also does the same if someone inadvertently
deletes one of the pods, leaving the node without the DaemonSet’s pod. Like a Replica-
Set, a DaemonSet creates the pod from the pod template configured in it.

Using a DaemonSet to run pods only on certain nodes
A DaemonSet deploys pods to all nodes in the cluster, unless you specify that the pods
should only run on a subset of all the nodes. This is done by specifying the node-
Selector property in the pod template, which is part of the DaemonSet definition
(similar to the pod template in a ReplicaSet or ReplicationController).

A node selector in a DaemonSet is similar—it defines the nodes the DaemonSet must
deploy its pods to.

```yaml
apiVersion: apps/v1beta2
kind: DaemonSet
metadata:
  name: ssd-monitor
spec:
  selector:
    matchLabels:
      app: ssd-monitor
  template:
    metadata:
      labels:
        app: ssd-monitor
  spec:
    nodeSelector:
      disk: ssd
    containers:
    - name: main
      image: luksa/ssd-monitor
```

```bash
# create daemon set
$ kubectl create -f ds.yaml

# list daemon sets
$ kubectl get ds

# list nodes
$ kubectl get node

# add label to node
$ kubectl label node NODE_NAME disk=ssd

# list pods
$ kubectl get pods

# change label
$ kubectl label node NODE_NAME disk=hdd --overwrite
```



# Jobs

Kubernetes includes support for this through the Job resource, which is similar to the
other resources we’ve discussed in this chapter, but it allows you to run a pod whose
container isn’t restarted when the process running inside finishes successfully. Once it
does, the pod is considered complete.







# Services


```yaml

```

```bash
$ kubectl exec POD_NAME -- curl -s http://CLUSTER_IP_OF_SERVICE
```


The kubectl exec command allows you to remotely run arbitrary commands inside
an existing container of a pod. This comes in handy when you want to examine the contents, state, and/or environment of a container.


## Session Affinity

If, on the other hand, you want all requests made by a certain client to be redirected
to the same pod every time, you can set the service’s sessionAffinity property
to ClientIP (instead of None, which is the default).

apiVersion: v1
kind: Service
spec:
  sessionAffinity: ClientIP

This makes the service proxy redirect all requests originating from the same client IP to the same pod.


Your service exposes only a single port, but services can also support multiple ports. For
example, if your pods listened on two ports—let’s say 8080 for HTTP and 8443 for
HTTPS—you could use a single service to forward both port 80 and 443 to the pod’s
ports 8080 and 8443. You don’t need to create two different services in such cases. Using
a single, multi-port service exposes all the service’s ports through a single cluster IP.

The label selector applies to the service as a whole—it can’t be configured
for each port individually. If you want different ports to map to different
subsets of pods, you need to create two services.

apiVersion: v1
kind: Service
metadata:
  name: kubia
spec:
ports:
  - name: http
    port: 80
    targetPort: 8080
  - name: https
    port: 443
    targetPort: 8443
  selector:
    app: kubia






## Discovering Services

By creating a service, you now have a single and stable IP address and port that you
can hit to access your pods. This address will remain unchanged throughout the
whole lifetime of the service. Pods behind this service may come and go, their IPs may
change, their number can go up or down, but they’ll always be accessible through the
service’s single and constant IP address.

When a pod is started, Kubernetes initializes a set of environment variables pointing
to each service that exists at that moment. If you create the service before creating the
client pods, processes in those pods can get the IP address and port of the service by
inspecting their environment variables.

```
$ kubectl exec kubia-3inly env
...
KUBIA_SERVICE_HOST=10.111.249.153
KUBIA_SERVICE_PORT=80
```

Remember in chapter 3 when you listed pods in the kube-system namespace? One of
the pods was called kube-dns. The kube-system namespace also includes a corresponding
service with the same name.

As the name suggests, the pod runs a DNS server, which all other pods running in
the cluster are automatically configured to use (Kubernetes does that by modifying
each container’s /etc/resolv.conf file). Any DNS query performed by a process running
in a pod will be handled by Kubernetes’ own DNS server, which knows all the services
running in your system.

Each service gets a DNS entry in the internal DNS server, and client pods that know
the name of the service can access it through its fully qualified domain name (FQDN)
instead of resorting to environment variables.






# Volumes


Each container in a pod has its own isolated filesystem because the filesystem comes from the container’s image.

Every new container starts off with the exact set of files that was added to the image at build time.

Containers in a pod get restarted (either because the process died or because the liveness probe signaled to Kubernetes that the container wasn’t healthy anymore) and you’ll realize that the new container will not see anything that was written to the filesystem by the previous container, even though the newly started container runs in the same pod.

In certain scenarios you want to preserve the directories that hold actual data.


Kubernetes provides this by defining storage volumes. They aren’t top-level resources
like pods, but are instead defined as a part of a pod and share the same lifecycle as the
pod. This means a volume is created when the pod is started and is destroyed when
the pod is deleted. Because of this, a volume’s contents will persist across container
restarts. After a container is restarted, the new container can see all the files that were
written to the volume by the previous container. Also, if a pod contains multiple containers,
the volume can be used by all of them at once.

Kubernetes volumes are a component of a pod and are thus defined in the pod’s specification—
much like containers. They aren’t a standalone Kubernetes object and cannot
be created or deleted on their own. A volume is available to all containers in the
pod, but it must be mounted in each container that needs to access it. In each container,
you can mount the volume in any location of its filesystem.

By mounting the same volume into two containers,
they can operate on the same files.

A volume is bound to the lifecycle of a pod and will stay in existence only while the
pod exists, but depending on the volume type, the volume’s files may remain intact
even after the pod and volume disappear, and can later be mounted into a new volume.

A wide variety of volume types is available. Several are generic, while others are specific
to the actual storage technologies used underneath.


- emptyDir: A simple empty directory used for storing transient data.
- hostPath: Used for mounting directories from the worker node’s filesystem into the pod.
- gitRepo: A volume initialized by checking out the contents of a Git repository.
- nfs: An NFS share mounted into the pod.
- gcePersistentDisk. awsElasticBlockStore, azureDisk: Used for mounting cloud provider-specific storage.
- persistentVolumeClaim: A way to use a pre- or dynamically provisioned persistent storage.



# PersistentVolumes









# Deployments

Kubernetes provides a Deployment resource that sits on top of ReplicaSets and enables declarative application updates.

When you create a Deployment, a ReplicaSet resource is created underneath (eventually more of them).

When using a Deployment, the actual pods are created and managed by the Deployment’s ReplicaSets, not by the Deployment directly.

A Deployment is backed by a ReplicaSet, which supervises the deployment’s pods.

Creating a Deployment isn’t that different from creating a ReplicationController. A
Deployment is also composed of a label selector, a desired replica count, and a pod
template. In addition to that, it also contains a field, which specifies a deployment
strategy that defines how an update should be performed when the Deployment
resource is modified.

As we said earlier, a Deployment
doesn’t manage pods directly. Instead, it creates ReplicaSets and leaves the managing
to them, so let’s look at the ReplicaSet created by your Deployment:

The ReplicaSet’s name also contains the hash value of its pod template. As you’ll see
later, a Deployment creates multiple ReplicaSets—one for each version of the pod
template. Using the hash value of the pod template like this allows the Deployment
to always use the same (possibly existing) ReplicaSet for a given version of the pod
template.




```yaml
apiVersion: apps/v1beta1
kind: Deployment
metadata:
  name: kubia
spec:
  replicas: 3
  template:
    metadata:
      name: kubia
      labels:
      app: kubia
    spec:
      containers:
      - image: luksa/kubia:v1
        name: nodejs
```


```bash
# create a deployment
$ kubectl create -f kubia-deployment-v1.yaml --record

# list deployments
$ kubectl get deployment

$ kubectl describe deployment

# check status of deployment
$ kubectl rollout status deployment DEPLOYMENT_NAME
```

Be sure to include the --record command-line option when creating it.
This records the command in the revision history, which will be useful later.


## Update Deployment

Now compare this to how you’re about to update a Deployment. The only thing
you need to do is modify the pod template defined in the Deployment resource and
Kubernetes will take all the steps necessary to get the actual system state to what’s
defined in the resource. Similar to scaling a ReplicationController or ReplicaSet up or
down, all you need to do is reference a new image tag in the Deployment’s pod template
and leave it to Kubernetes to transform your system so it matches the new
desired state.

How this new state should be achieved is governed by the deployment strategy configured
on the Deployment itself. The default strategy is to perform a rolling update (the
strategy is called RollingUpdate). The alternative is the Recreate strategy, which
deletes all the old pods at once and then creates new ones, similar to modifying a
ReplicationController’s pod template and then deleting all the pods (we talked about
this in section 9.1.1).

The Recreate strategy causes all old pods to be deleted before the new ones are
created. Use this strategy when your application doesn’t support running multiple versions
in parallel and requires the old version to be stopped completely before the
new one is started. This strategy does involve a short period of time when your app
becomes completely unavailable.

The RollingUpdate strategy, on the other hand, removes old pods one by one,
while adding new ones at the same time, keeping the application available throughout
the whole process, and ensuring there’s no drop in its capacity to handle requests.
This is the default strategy. The upper and lower limits for the number of pods above
or below the desired replica count are configurable. You should use this strategy only
when your app can handle running both the old and new version at the same time.

If you’d like to track the update process as it progresses, first run the curl loop again
in another terminal to see what’s happening with the requests (don’t forget to replace
the IP with the actual external IP of your service):

$ while true; do curl http://130.211.109.222; done

To trigger the actual rollout, you’ll change the image used in the single pod container
to luksa/kubia:v2. Instead of editing the whole YAML of the Deployment object or
using the patch command to change the image, you’ll use the kubectl set image
command, which allows changing the image of any resource that contains a container
(ReplicationControllers, ReplicaSets, Deployments, and so on). You’ll use it to modify
your Deployment like this:

$ kubectl set image deployment kubia nodejs=luksa/kubia:v2

When you execute this command, you’re updating the kubia Deployment’s pod template
so the image used in its nodejs container is changed to luksa/kubia:v2 (from
:v1).

By changing the pod template in your Deployment
resource, you’ve updated your app to a newer version
An additional ReplicaSet
was created and it was then scaled up slowly, while the previous ReplicaSet was scaled
down to zero









# Stateful Sets

ReplicaSets create multiple pod replicas from a single pod template. These replicas
don’t differ from each other, apart from their name and IP address. If the pod template
includes a volume, which refers to a specific PersistentVolumeClaim, all replicas
of the ReplicaSet will use the exact same PersistentVolumeClaim and therefore the
same PersistentVolume bound by the claim.

Because the reference to the claim is in the pod template, which is used to stamp out
multiple pod replicas, you can’t make each replica use its own separate Persistent-
VolumeClaim. You can’t use a ReplicaSet to run a distributed data store, where each
instance needs its own separate storage—at least not by using a single ReplicaSet.


In addition to storage, certain clustered applications also require that each instance
has a long-lived stable identity. Pods can be killed from time to time and replaced with  new ones. When a ReplicaSet replaces a pod, the new pod is a completely new pod
with a new hostname and IP, although the data in its storage volume may be that of
the killed pod. For certain apps, starting up with the old instance’s data but with a
completely new network identity may cause problems.
Why do certain apps mandate a stable network identity? This requirement is
fairly common in distributed stateful applications. Certain apps require the administrator
to list all the other cluster members and their IP addresses (or hostnames) in
each member’s configuration file. But in Kubernetes, every time a pod is rescheduled,
the new pod gets both a new hostname and a new IP address, so the whole
application cluster would have to be reconfigured every time one of its members is
rescheduled.



Instead of using a ReplicaSet to run these types of pods, you create a StatefulSet
resource, which is specifically tailored to applications where instances of the application
must be treated as non-fungible individuals, with each one having a stable name
and state.

Pod replicas managed by a ReplicaSet or ReplicationController are much like cattle.
Because they’re mostly stateless, they can be replaced with a completely new pod
replica at any time. Stateful pods require a different approach. When a stateful pod
instance dies (or the node it’s running on fails), the pod instance needs to be resurrected
on another node, but the new instance needs to get the same name, network
identity, and state as the one it’s replacing. This is what happens when the pods are
managed through a StatefulSet.

A StatefulSet makes sure pods are rescheduled in such a way that they retain their
identity and state. It also allows you to easily scale the number of pets up and down. A
StatefulSet, like a ReplicaSet, has a desired replica count field that determines how
many pets you want running at that time. Similar to ReplicaSets, pods are created from
a pod template specified as part of the StatefulSet (remember the cookie-cutter analogy?).
But unlike pods created by ReplicaSets, pods created by the StatefulSet aren’t
exact replicas of each other. Each can have its own set of volumes—in other words,
storage (and thus persistent state)—which differentiates it from its peers. Pet pods
also have a predictable (and stable) identity instead of each new pod instance getting
a completely random one.

Each pod created by a StatefulSet is assigned an ordinal index (zero-based), which
is then used to derive the pod’s name and hostname, and to attach stable storage to
the pod. The names of the pods are thus predictable, because each pod’s name is
derived from the StatefulSet’s name and the ordinal index of the instance. Rather
than the pods having random names, they’re nicely organized, as shown in the next
figure.

INTRODUCING THE GOVERNING SERVICE
But it’s not all about the pods having a predictable name and hostname. Unlike regular
pods, stateful pods sometimes need to be addressable by their hostname, whereas
stateless pods usually don’t. After all, each stateless pod is like any other. When you
need one, you pick any one of them. But with stateful pods, you usually want to operate
on a specific pod from the group, because they differ from each other (they hold
different state, for example).
For this reason, a StatefulSet requires you to create a corresponding governing
headless Service that’s used to provide the actual network identity to each pod.
Through this Service, each pod gets its own DNS entry, so its peers and possibly other
clients in the cluster can address the pod by its hostname. For example, if the governing
Service belongs to the default namespace and is called foo, and one of the podsis called A-0, you can reach the pod through its fully qualified domain name, which
is a-0.foo.default.svc.cluster.local. You can’t do that with pods managed by a
ReplicaSet.
Additionally, you can also use DNS to look up all the StatefulSet’s pods’ names by
looking up SRV records for the foo.default.svc.cluster.local domain. We’ll
explain SRV records in section 10.4 and learn how they’re used to discover members
of a StatefulSet.

When a pod instance managed by a StatefulSet disappears (because the node the pod
was running on has failed, it was evicted from the node, or someone deleted the pod
object manually), the StatefulSet makes sure it’s replaced with a new instance—similar
to how ReplicaSets do it. But in contrast to ReplicaSets, the replacement pod gets the
same name and hostname as the pod that has disappeared (this distinction between
ReplicaSets and StatefulSets is illustrated in figure 10.6).

Scaling the StatefulSet creates a new pod instance with the next unused ordinal index.
If you scale up from two to three instances, the new instance will get index 2 (the existing
instances obviously have indexes 0 and 1).
The nice thing about scaling down a StatefulSet is the fact that you always know
what pod will be removed. Again, this is also in contrast to scaling down a ReplicaSet,
where you have no idea what instance will be deleted, and you can’t even specify
which one you want removed first (but this feature may be introduced in the future).
Scaling down a StatefulSet always removes the instances with the highest ordinal index
first (shown in figure 10.7). This makes the effects of a scale-down predictable.

Because certain stateful applications don’t handle rapid scale-downs nicely, Stateful-
Sets scale down only one pod instance at a time. A distributed data store, for example,
may lose data if multiple nodes go down at the same time. For example, if a replicated
data store is configured to store two copies of each data entry, in cases where two
nodes go down at the same time, a data entry would be lost if it was stored on exactly
those two nodes. If the scale-down was sequential, the distributed data store has time
to create an additional replica of the data entry somewhere else to replace the (single)
lost copy.
For this exact reason, StatefulSets also never permit scale-down operations if any of
the instances are unhealthy. If an instance is unhealthy, and you scale down by one at
the same time, you’ve effectively lost two cluster members at once.



